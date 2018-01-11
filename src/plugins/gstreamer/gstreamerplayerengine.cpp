#include "gstreamerplayerengine.h"
#include "gstutils.h"

#include <QTimer>

HS_BEGIN_NAMESPACE

Q_DECLARE_LOGGING_CATEGORY(__gst__)

static bool usePlaybinVolume()
{
    static enum { Yes, No, Unknown } status = Unknown;
    if (status == Unknown) {
        QByteArray v = qgetenv("HS_GSTREAMER_USE_PLAYBIN_VOLUME");
        bool value = !v.isEmpty() && v != "0" && v != "false";
        if (value)
            status = Yes;
        else
            status = No;
    }
    return status == Yes;
}

#if GST_CHECK_VERSION(1,0,0)
static GstPadProbeReturn block_pad_cb(GstPad *pad, GstPadProbeInfo *info, gpointer user_data)
#else
static void block_pad_cb(GstPad *pad, gboolean blocked, gpointer user_data)
#endif
{
    Q_UNUSED(pad);
#if GST_CHECK_VERSION(1,0,0)
    Q_UNUSED(info);
    Q_UNUSED(user_data);
    return GST_PAD_PROBE_OK;
#else
    qCInfo(__gst__) << "block_pad_cb, blocked:" << blocked;
    if (blocked && user_data) {
        GstreamerPlayerEngine *engine = reinterpret_cast<GstreamerPlayerEngine*>(user_data);
        QMetaObject::invokeMethod(engine, "finishVideoOutputChange", Qt::QueuedConnection);
    }
#endif
}

GstreamerPlayerEngine::GstreamerPlayerEngine(QObject *parent) :
    MultimediaEngine(parent),
    m_state(MultimediaPlayer::StoppedState),
    m_pendingState(MultimediaPlayer::StoppedState),
    m_volume(100),
    m_playbackRate(1.0),
    m_muted(false),
    m_audioAvailable(false),
    m_videoAvailable(false),
    m_seekable(false),
    m_lastPosition(0),
    m_duration(-1),
    m_durationQueries(0),
    m_displayPrerolledFrame(true),
    m_playbin(0),
    m_audioSink(0),
    m_volumeElement(0),
    m_videoSink(0),
    m_videoOutputBin(0),
    m_videoIdentity(0),
    m_pendingVideoSink(0),
    m_nullVideoSink(0),
    m_bus(0),
    m_busHelper(0),
    m_videoOutput(0),
    m_renderer(0),
    m_sourceType(UnknownSrc),
    m_everPlayed(false),
    m_isLiveSource(false),
    pad_probe_id(0)
{
    m_playbin = gst_element_factory_make(HS_GSTREAMER_PLAYBIN_ELEMENT_NAME, NULL);
    if (m_playbin) {
        // TODO: Check Flags
        gint flags;
        g_object_get(G_OBJECT(m_playbin), "flags", &flags, NULL);
        QByteArray envFlags = qgetenv("HS_GSTREAMER_PLAYBIN_FLAGS");
        if (!envFlags.isEmpty()) {
            flags |= envFlags.toInt();
#if GST_CHECK_VERSION(1,0,0)
        } else {
            // GST_PLAY_FLAG_NATIVE_VIDEO only allow native video formats, this omits configuration of ffmpegcolorspace and videoscale.
            flags |= GST_PLAY_FLAG_NATIVE_VIDEO;
#endif
        }
        g_object_set(G_OBJECT(m_playbin), "flags", flags, NULL);

        // AudioSink
        const QByteArray envAudioSink = qgetenv("HS_GSTREAMER_PLAYBIN_AUDIOSINK");
        GstElement *audioSink = gst_element_factory_make(envAudioSink.isEmpty() ? "autoaudiosink" : envAudioSink,
                                                         "audiosink");
        if (audioSink) {
            // TODO: Check Volume
            if (usePlaybinVolume()) {
                m_audioSink = audioSink;
                m_volumeElement = m_playbin;
            } else {
                m_volumeElement = gst_element_factory_make("volume", "volumeelement");
                if (m_volumeElement) {
                    m_audioSink = gst_bin_new("audio-output-bin");

                    gst_bin_add_many(GST_BIN(m_audioSink), m_volumeElement, audioSink, NULL);
                    gst_element_link(m_volumeElement, audioSink);

                    GstPad *pad = gst_element_get_static_pad(m_volumeElement, "sink");
                    gst_element_add_pad(GST_ELEMENT(m_audioSink), gst_ghost_pad_new("sink", pad));
                    gst_object_unref(GST_OBJECT(pad));
                } else {
                    m_audioSink = audioSink;
                    m_volumeElement = m_playbin;
                }
            }

            g_object_set(G_OBJECT(m_playbin), "audio-sink", m_audioSink, NULL);
//            addAudioBufferProbe();
        }

        // 这是一个哑巴element仅仅把输入数据不加修改的传过来。它也有一些有用的调试函数，例如偏移量和时间戳检测，或者丢弃缓冲。想要进一步了解请阅读它的文档。
        m_videoIdentity = gst_element_factory_make("identity", NULL); // floating ref
        // fakesink仅仅简单的抛弃所有输入的数据。在调试过程中它是很有用的，你可以用它来取代你想使用的sink来确保不会出现意外。它在gst-lauch命令行并出现-v参数后会显得非常冗余，如果你觉得无用信息太多，那么可以设置silent属性。
        m_nullVideoSink = gst_element_factory_make("fakesink", NULL); // m_nullVideoSink
        g_object_set(G_OBJECT(m_nullVideoSink), "sync", true, NULL);
        gst_object_ref(GST_OBJECT(m_nullVideoSink));

        m_videoOutputBin = gst_bin_new("video-output-bin");
        // might not get a parent, take ownership to avoid leak
        hs_gst_object_ref_sink(GST_OBJECT(m_videoOutputBin));
        gst_bin_add_many(GST_BIN(m_videoOutputBin), m_videoIdentity, m_nullVideoSink, NULL);
        gst_element_link(m_videoIdentity, m_nullVideoSink);

        m_videoSink = m_nullVideoSink;

        // add ghostpads
        GstPad *pad = gst_element_get_static_pad(m_videoIdentity, "sink");
        gst_element_add_pad(GST_ELEMENT(m_videoOutputBin), gst_ghost_pad_new("sink", pad));
        gst_object_unref(GST_OBJECT(pad));

        if (m_playbin != 0) {
            // Sort out messages
            m_bus = gst_element_get_bus(m_playbin);
            m_busHelper = new GstreamerBusHelper(m_bus, this);
            m_busHelper->installMessageFilter(this);

//            g_object_set(G_OBJECT(m_playbin), "video-sink", m_videoOutputBin, NULL);

//            g_signal_connect(G_OBJECT(m_playbin), "source-setup", G_CALLBACK(playbinNotifySource), this); // notify::source
//            g_signal_connect(G_OBJECT(m_playbin), "element-setup",  G_CALLBACK(handleElementAdded), this);  // element-added

            if (usePlaybinVolume()) {
                updateVolume();
                updateMuted();
                g_signal_connect(G_OBJECT(m_playbin), "notify::volume", G_CALLBACK(handleVolumeChange), this);
                g_signal_connect(G_OBJECT(m_playbin), "notify::mute", G_CALLBACK(handleMutedChange), this);
            }

//            g_signal_connect(G_OBJECT(m_playbin), "video-changed", G_CALLBACK(handleStreamsChange), this);
//            g_signal_connect(G_OBJECT(m_playbin), "audio-changed", G_CALLBACK(handleStreamsChange), this);
//            g_signal_connect(G_OBJECT(m_playbin), "text-changed", G_CALLBACK(handleStreamsChange), this);
        }

    }

}

GstreamerPlayerEngine::~GstreamerPlayerEngine()
{
    if (m_playbin) {
        stop();

//        removeVideoBufferProbe();
//        removeAudioBufferProbe();

        delete m_busHelper;
        gst_object_unref(GST_OBJECT(m_bus));
        gst_object_unref(GST_OBJECT(m_playbin));
#if !GST_CHECK_VERSION(1,0,0)
//        gst_object_unref(GST_OBJECT(m_colorSpace));
#endif
        gst_object_unref(GST_OBJECT(m_nullVideoSink));
        gst_object_unref(GST_OBJECT(m_videoOutputBin));
    }
}

GstElement *GstreamerPlayerEngine::playbin() const
{
    return m_playbin;
}

qint64 GstreamerPlayerEngine::duration() const
{
    return m_duration;
}

qint64 GstreamerPlayerEngine::position() const
{
    gint64      position = 0;
    if (m_playbin && hs_gst_element_query_position(m_playbin, GST_FORMAT_TIME, &position))
        m_lastPosition = position / 1000000;
    return m_lastPosition;
}

qreal GstreamerPlayerEngine::playbackRate() const
{
    return m_playbackRate;
}

void GstreamerPlayerEngine::setPlaybackRate(qreal rate)
{
    if (!qFuzzyCompare(m_playbackRate, rate)) {
        qCInfo(__gst__, "[ SetPlaybackRate ] = %f", rate);

        m_playbackRate = rate;
        if (m_playbin && m_seekable) {
            gst_element_seek(m_playbin, rate, GST_FORMAT_TIME,
                             GstSeekFlags(GST_SEEK_FLAG_FLUSH),
                             GST_SEEK_TYPE_NONE,0,
                             GST_SEEK_TYPE_NONE,0 );
        }
        emit playbackRateChanged(m_playbackRate);
    }
}

int GstreamerPlayerEngine::volume() const
{
    return m_volume;
}

bool GstreamerPlayerEngine::isMuted() const
{
    return m_muted;
}

bool GstreamerPlayerEngine::isAudioAvailable() const
{
    return m_audioAvailable;
}

bool GstreamerPlayerEngine::isVideoAvailable() const
{
    return m_videoAvailable;
}

void GstreamerPlayerEngine::setVideoRenderer(QObject *videoOutput)
{
    qCInfo(__gst__) << "[ SetVideoRenderer ] =" << videoOutput;

    if (m_videoOutput != videoOutput) {
        if (m_videoOutput) {
            disconnect(m_videoOutput, SIGNAL(sinkChanged()),
                       this, SLOT(updateVideoRenderer()));
            disconnect(m_videoOutput, SIGNAL(readyChanged(bool)),
                   this, SLOT(updateVideoRenderer()));

            m_busHelper->removeMessageFilter(m_videoOutput);
        }

        m_videoOutput = videoOutput;

        if (m_videoOutput) {
            connect(m_videoOutput, SIGNAL(sinkChanged()),
                    this, SLOT(updateVideoRenderer()));
            connect(m_videoOutput, SIGNAL(readyChanged(bool)),
                   this, SLOT(updateVideoRenderer()));

            m_busHelper->installMessageFilter(m_videoOutput);
        }
    }

    GstreamerVideoRendererInterface* renderer = qobject_cast<GstreamerVideoRendererInterface*>(videoOutput);

    m_renderer = renderer;

#ifdef DEBUG_VO_BIN_DUMP
    gst_debug_bin_to_dot_file_with_ts(GST_BIN(m_playbin),
                                  GstDebugGraphDetails(GST_DEBUG_GRAPH_SHOW_ALL /* GST_DEBUG_GRAPH_SHOW_MEDIA_TYPE | GST_DEBUG_GRAPH_SHOW_NON_DEFAULT_PARAMS | GST_DEBUG_GRAPH_SHOW_STATES*/),
                                  "playbin_set");
#endif

    GstElement *videoSink = 0;
    if (m_renderer && m_renderer->isReady())
        videoSink = m_renderer->videoSink();

    if (!videoSink)
        videoSink = m_nullVideoSink;

    qCInfo(__gst__) << "Set video output:" << videoOutput;
    qCInfo(__gst__) << "Current sink:" << (m_videoSink ? GST_ELEMENT_NAME(m_videoSink) : "") <<  m_videoSink
                     << "pending:" << (m_pendingVideoSink ? GST_ELEMENT_NAME(m_pendingVideoSink) : "") << m_pendingVideoSink
                     << "new sink:" << (videoSink ? GST_ELEMENT_NAME(videoSink) : "") << videoSink;

    if (m_pendingVideoSink == videoSink ||
        (m_pendingVideoSink == 0 && m_videoSink == videoSink)) {
        qCInfo(__gst__) << "Video sink has not changed, skip video output reconfiguration";
        return;
    }

    qCInfo(__gst__) << "Reconfigure video output";

    if (m_state == MultimediaPlayer::StoppedState) {
        qCInfo(__gst__) << "The pipeline has not started yet, pending state:" << m_pendingState;

        //the pipeline has not started yet
        flushVideoProbes();
        m_pendingVideoSink = 0;
        gst_element_set_state(m_videoSink, GST_STATE_NULL);
        gst_element_set_state(m_playbin, GST_STATE_NULL);

//#if !GST_CHECK_VERSION(1,0,0)
//        if (m_usingColorspaceElement) {
//            gst_element_unlink(m_colorSpace, m_videoSink);
//            gst_bin_remove(GST_BIN(m_videoOutputBin), m_colorSpace);
//        } else {
//            gst_element_unlink(m_videoIdentity, m_videoSink);
//        }
//#endif

        removeVideoBufferProbe();

        gst_bin_remove(GST_BIN(m_videoOutputBin), m_videoSink);

        m_videoSink = videoSink;

        gst_bin_add(GST_BIN(m_videoOutputBin), m_videoSink);

        bool linked = gst_element_link(m_videoIdentity, m_videoSink);
//#if !GST_CHECK_VERSION(1,0,0)
//        m_usingColorspaceElement = false;
//        if (!linked) {
//            m_usingColorspaceElement = true;
//            qCInfo(__gst__) << "Failed to connect video output, inserting the colorspace element.";
//            gst_bin_add(GST_BIN(m_videoOutputBin), m_colorSpace);
//            linked = gst_element_link_many(m_videoIdentity, m_colorSpace, m_videoSink, NULL);
//        }
//#endif

        if (!linked)
            qWarning() << "Linking video output element failed";

        if (g_object_class_find_property(G_OBJECT_GET_CLASS(m_videoSink), "show-preroll-frame") != 0) {
            gboolean value = m_displayPrerolledFrame;
            g_object_set(G_OBJECT(m_videoSink), "show-preroll-frame", value, NULL);
        }

        addVideoBufferProbe();

        switch (m_pendingState) {
        case MultimediaPlayer::PausedState:
            gst_element_set_state(m_playbin, GST_STATE_PAUSED);
            break;
        case MultimediaPlayer::PlayingState:
            gst_element_set_state(m_playbin, GST_STATE_PLAYING);
            break;
        default:
            break;
        }

        resumeVideoProbes();

    } else {
        if (m_pendingVideoSink) {
            qCInfo(__gst__) << "already waiting for pad to be blocked, just change the pending sink";

            m_pendingVideoSink = videoSink;
            return;
        }

        m_pendingVideoSink = videoSink;

        qCInfo(__gst__) << "Blocking the video output pad...";

        //block pads, async to avoid locking in paused state
        GstPad *srcPad = gst_element_get_static_pad(m_videoIdentity, "src");
#if GST_CHECK_VERSION(1,0,0)
        this->pad_probe_id = gst_pad_add_probe(srcPad, (GstPadProbeType)(GST_PAD_PROBE_TYPE_BUFFER | GST_PAD_PROBE_TYPE_BLOCKING), block_pad_cb, this, NULL);
#else
        gst_pad_set_blocked_async(srcPad, true, &block_pad_cb, this);
#endif
        gst_object_unref(GST_OBJECT(srcPad));

        //Unpause the sink to avoid waiting until the buffer is processed
        //while the sink is paused. The pad will be blocked as soon as the current
        //buffer is processed.
        if (m_state == MultimediaPlayer::PausedState) {
            qCInfo(__gst__) << "Starting video output to avoid blocking in paused state...";

            gst_element_set_state(m_videoSink, GST_STATE_PLAYING);
        }
    }
}

bool GstreamerPlayerEngine::isSeekable() const
{
    return m_seekable;
}

bool GstreamerPlayerEngine::isLiveSource() const
{
    return m_isLiveSource;
}

// This function is similar to stop(),
// but does not set m_everPlayed, m_lastPosition,
// and setSeekable() values.
void GstreamerPlayerEngine::endOfMediaReset()
{
    if (m_renderer)
        m_renderer->stopRenderer();

    flushVideoProbes();
    gst_element_set_state(m_playbin, GST_STATE_NULL);

    MultimediaPlayer::State oldState = m_state;
    m_pendingState = m_state = MultimediaPlayer::StoppedState;

    finishVideoOutputChange();

    if (oldState != m_state)
        emit stateChanged(m_state);
}

bool GstreamerPlayerEngine::processBusMessage(const GstreamerMessage &message)
{
    GstMessage* gm = message.rawMessage();
    if (gm) {
        //tag message comes from elements inside playbin, not from playbin itself
        if (GST_MESSAGE_TYPE(gm) == GST_MESSAGE_TAG) {
            GstTagList *tag_list;
            gst_message_parse_tag(gm, &tag_list);

            QMap<QByteArray, QVariant> newTags = GstUtils::gstTagListToMap(tag_list);
            QMap<QByteArray, QVariant>::const_iterator it = newTags.constBegin();
            for ( ; it != newTags.constEnd(); ++it) {
                qCDebug(__gst__) << qUtf8Printable(QString("[ TAG ] [ %1 ] : %2") \
                                                   .arg(QString(it.key())) \
                                                   .arg(it.value().toString()));

                m_tags.insert(it.key(), it.value()); // overwrite existing tags
            }

            gst_tag_list_free(tag_list);

            emit tagsChanged();
        } else if (GST_MESSAGE_TYPE(gm) == GST_MESSAGE_DURATION) {
            updateDuration();
        }

        if (m_sourceType == MMSSrc && qstrcmp(GST_OBJECT_NAME(GST_MESSAGE_SRC(gm)), "source") == 0) {
            qCDebug(__gst__) << "[ Message ] from MMSSrc: " << GST_MESSAGE_TYPE(gm);
        } else if (m_sourceType == RTSPSrc && qstrcmp(GST_OBJECT_NAME(GST_MESSAGE_SRC(gm)), "source") == 0) {
            qCDebug(__gst__) << "[ Message ] from RTSPSrc: " << GST_MESSAGE_TYPE(gm);
        } else {
            qCDebug(__gst__) << "[ Message ] from " << GST_OBJECT_NAME(GST_MESSAGE_SRC(gm)) << ":" << GST_MESSAGE_TYPE(gm);
        }

        if (GST_MESSAGE_TYPE(gm) == GST_MESSAGE_BUFFERING) {
            int progress = 0;
            gst_message_parse_buffering(gm, &progress);
            emit bufferingProgressChanged(progress);
        }

        bool handlePlaybin2 = false;
        if (GST_MESSAGE_SRC(gm) == GST_OBJECT_CAST(m_playbin)) {
            switch (GST_MESSAGE_TYPE(gm))  {
            case GST_MESSAGE_STATE_CHANGED:
                {
                    GstState    oldState;
                    GstState    newState;
                    GstState    pending;

                    gst_message_parse_state_changed(gm, &oldState, &newState, &pending);

                    QStringList states;
                    states << "GST_STATE_VOID_PENDING" <<  "GST_STATE_NULL" << "GST_STATE_READY" << "GST_STATE_PAUSED" << "GST_STATE_PLAYING";
                    qCInfo(__gst__) << qPrintable(QString("[ State Changed ] : old: %1  new: %2  pending: %3") \
                                                  .arg(states[oldState]) \
                                                  .arg(states[newState]) \
                                                  .arg(states[pending]));

                    switch (newState) {
                    case GST_STATE_VOID_PENDING:
                    case GST_STATE_NULL:
                        setSeekable(false);
                        finishVideoOutputChange();
                        if (m_state != MultimediaPlayer::StoppedState)
                            emit stateChanged(m_state = MultimediaPlayer::StoppedState);
                        break;
                    case GST_STATE_READY:
                        setSeekable(false);
                        if (m_state != MultimediaPlayer::StoppedState)
                            emit stateChanged(m_state = MultimediaPlayer::StoppedState);
                        break;
                    case GST_STATE_PAUSED:
                    {
                        MultimediaPlayer::State prevState = m_state;
                        m_state = MultimediaPlayer::PausedState;

                        //check for seekable
                        if (oldState == GST_STATE_READY) {

// TODO:
//                            getStreamsInfo();
//                            updateVideoResolutionTag();

                            //gstreamer doesn't give a reliable indication the duration
                            //information is ready, GST_MESSAGE_DURATION is not sent by most elements
                            //the duration is queried up to 5 times with increasing delay
                            m_durationQueries = 5;
                            // This should also update the seekable flag.
                            updateDuration();

                            if (!qFuzzyCompare(m_playbackRate, qreal(1.0))) {
                                qreal rate = m_playbackRate;
                                m_playbackRate = 1.0;
                                setPlaybackRate(rate);
                            }
                        }

                        if (m_state != prevState)
                            emit stateChanged(m_state);

                        break;
                    }
                    case GST_STATE_PLAYING:
                        m_everPlayed = true;
                        if (m_state != MultimediaPlayer::PlayingState) {
                            emit stateChanged(m_state = MultimediaPlayer::PlayingState);

                            // For rtsp streams duration information might not be available
                            // until playback starts.
                            if (m_duration <= 0) {
                                m_durationQueries = 5;
                                updateDuration();
                            }
                        }

                        break;
                    }
                }
                break;

            case GST_MESSAGE_EOS:
                emit playbackFinished();
                break;

            case GST_MESSAGE_TAG:
            case GST_MESSAGE_STREAM_STATUS:
            case GST_MESSAGE_UNKNOWN:
                break;
            case GST_MESSAGE_ERROR: {
                    GError *err;
                    gchar *debug;
                    gst_message_parse_error(gm, &err, &debug);
                    if (err->domain == GST_STREAM_ERROR && err->code == GST_STREAM_ERROR_CODEC_NOT_FOUND)
                        processInvalidMedia(MultimediaPlayer::FormatError, tr("Cannot play stream of type: <unknown>"));
                    else
                        processInvalidMedia(MultimediaPlayer::ResourceError, QString::fromUtf8(err->message));
                    qWarning() << "[ Error ]:" << QString::fromUtf8(err->message);
                    g_error_free(err);
                    g_free(debug);
                }
                break;
            case GST_MESSAGE_WARNING:
                {
                    GError *err;
                    gchar *debug;
                    gst_message_parse_warning (gm, &err, &debug);
                    qWarning() << "[ Warning ]:" << QString::fromUtf8(err->message);
                    g_error_free (err);
                    g_free (debug);
                }
                break;
            case GST_MESSAGE_INFO:
            {
                GError *err;
                gchar *debug;
                gst_message_parse_info (gm, &err, &debug);
                qCInfo(__gst__) << "[ Info ]:" << QString::fromUtf8(err->message);
                g_error_free (err);
                g_free (debug);
            }
                break;
            case GST_MESSAGE_BUFFERING:
            case GST_MESSAGE_STATE_DIRTY:
            case GST_MESSAGE_STEP_DONE:
            case GST_MESSAGE_CLOCK_PROVIDE:
            case GST_MESSAGE_CLOCK_LOST:
            case GST_MESSAGE_NEW_CLOCK:
            case GST_MESSAGE_STRUCTURE_CHANGE:
            case GST_MESSAGE_APPLICATION:
            case GST_MESSAGE_ELEMENT:
                break;
            case GST_MESSAGE_SEGMENT_START:
                {
                    const GstStructure *structure = gst_message_get_structure(gm);
                    qint64 position = g_value_get_int64(gst_structure_get_value(structure, "position"));
                    position /= 1000000;
                    m_lastPosition = position;
                    emit positionChanged(position);
                }
                break;
            case GST_MESSAGE_SEGMENT_DONE:
                break;
            case GST_MESSAGE_LATENCY:
#if GST_CHECK_VERSION(0,10,13)
            case GST_MESSAGE_ASYNC_START:
                break;
            case GST_MESSAGE_ASYNC_DONE:
            {
                gint64      position = 0;
                if (hs_gst_element_query_position(m_playbin, GST_FORMAT_TIME, &position)) {
                    position /= 1000000;
                    m_lastPosition = position;
                    qCInfo(__gst__, "Position : %lld ms", position);
                    emit positionChanged(position);
                }
                break;
            }
#if GST_CHECK_VERSION(0,10,23)
            case GST_MESSAGE_REQUEST_STATE:
#endif
#endif
            case GST_MESSAGE_ANY:
                break;
            default:
                break;
            }
        } else if (GST_MESSAGE_TYPE(gm) == GST_MESSAGE_ERROR) {
            GError *err;
            gchar *debug;
            gst_message_parse_error(gm, &err, &debug);
            // If the source has given up, so do we.
            if (qstrcmp(GST_OBJECT_NAME(GST_MESSAGE_SRC(gm)), "source") == 0) {
                bool everPlayed = m_everPlayed;
                // Try and differentiate network related resource errors from the others
                if (!m_request.url().isRelative() && m_request.url().scheme().compare(QLatin1String("file"), Qt::CaseInsensitive) != 0 ) {
                    if (everPlayed ||
                        (err->domain == GST_RESOURCE_ERROR && (
                         err->code == GST_RESOURCE_ERROR_BUSY ||
                         err->code == GST_RESOURCE_ERROR_OPEN_READ ||
                         err->code == GST_RESOURCE_ERROR_READ ||
                         err->code == GST_RESOURCE_ERROR_SEEK ||
                         err->code == GST_RESOURCE_ERROR_SYNC))) {
                        processInvalidMedia(MultimediaPlayer::NetworkError, QString::fromUtf8(err->message));
                    } else {
                        processInvalidMedia(MultimediaPlayer::ResourceError, QString::fromUtf8(err->message));
                    }
                }
                else
                    processInvalidMedia(MultimediaPlayer::ResourceError, QString::fromUtf8(err->message));
            } else if (err->domain == GST_STREAM_ERROR
                       && (err->code == GST_STREAM_ERROR_DECRYPT || err->code == GST_STREAM_ERROR_DECRYPT_NOKEY)) {
                processInvalidMedia(MultimediaPlayer::AccessDeniedError, QString::fromUtf8(err->message));
            } else {
                handlePlaybin2 = true;
            }
            if (!handlePlaybin2)
                qWarning() << "Error:" << QString::fromUtf8(err->message);
            g_error_free(err);
            g_free(debug);
        } else if (GST_MESSAGE_TYPE(gm) == GST_MESSAGE_ELEMENT
                   && qstrcmp(GST_OBJECT_NAME(GST_MESSAGE_SRC(gm)), "source") == 0
                   && m_sourceType == UDPSrc
                   && gst_structure_has_name(gst_message_get_structure(gm), "GstUDPSrcTimeout")) {
            //since udpsrc will not generate an error for the timeout event,
            //we need to process its element message here and treat it as an error.
            processInvalidMedia(m_everPlayed ? MultimediaPlayer::NetworkError : MultimediaPlayer::ResourceError,
                                tr("UDP source timeout"));
        } else {
            handlePlaybin2 = true;
        }

        if (handlePlaybin2) {
            if (GST_MESSAGE_TYPE(gm) == GST_MESSAGE_WARNING) {
                GError *err;
                gchar *debug;
                gst_message_parse_warning(gm, &err, &debug);
                if (err->domain == GST_STREAM_ERROR && err->code == GST_STREAM_ERROR_CODEC_NOT_FOUND)
                    emit error(int(MultimediaPlayer::FormatError), tr("Cannot play stream of type: <unknown>"));
                // GStreamer shows warning for HTTP playlists
                if (err && err->message)
                    qWarning() << "Warning:" << QString::fromUtf8(err->message);
                g_error_free(err);
                g_free(debug);
            } else if (GST_MESSAGE_TYPE(gm) == GST_MESSAGE_ERROR) {
                GError *err;
                gchar *debug;
                gst_message_parse_error(gm, &err, &debug);

                // Nearly all errors map to ResourceError
                MultimediaPlayer::Error qerror = MultimediaPlayer::ResourceError;
                if (err->domain == GST_STREAM_ERROR
                           && (err->code == GST_STREAM_ERROR_DECRYPT
                               || err->code == GST_STREAM_ERROR_DECRYPT_NOKEY)) {
                    qerror = MultimediaPlayer::AccessDeniedError;
                }
                processInvalidMedia(qerror, QString::fromUtf8(err->message));
                if (err && err->message)
                    qWarning() << "Error:" << QString::fromUtf8(err->message);

                g_error_free(err);
                g_free(debug);
            }
        }
    }

    return false;
}

// PUBLIC SLOTS
void GstreamerPlayerEngine::loadFromUri(const QNetworkRequest &request)
{
    qCInfo(__gst__) << "[ LoadFromUri ] =" << request.url().toString();

    m_request = request;
    m_duration = -1;
    m_lastPosition = 0;

    if (m_playbin) {
        m_tags.clear();
        emit tagsChanged();

        g_object_set(G_OBJECT(m_playbin), "uri", m_request.url().toEncoded().constData(), NULL);

//        if (!m_streamTypes.isEmpty()) {
//            m_streamProperties.clear();
//            m_streamTypes.clear();

//            emit streamsChanged();
//        }
    }

}

bool GstreamerPlayerEngine::play()
{
    qCInfo(__gst__) << "[ Play ] when" << m_state;

    m_everPlayed = false;
    if (m_playbin) {
        m_pendingState = MultimediaPlayer::PlayingState;
        if (gst_element_set_state(m_playbin, GST_STATE_PLAYING) == GST_STATE_CHANGE_FAILURE) {
            qCWarning(__gst__) << "[ GStreamer ] Unable to play -" << m_request.url().toString();
            m_pendingState = m_state = MultimediaPlayer::StoppedState;
            emit stateChanged(m_state);
        } else {
            resumeVideoProbes();
            return true;
        }
    }

    return false;
}

bool GstreamerPlayerEngine::pause()
{
    qCInfo(__gst__) << "[ Pause ] when" << m_state;

    if (m_playbin) {
        m_pendingState = MultimediaPlayer::PausedState;
        if (m_pendingVideoSink != 0)
            return true;

        if (gst_element_set_state(m_playbin, GST_STATE_PAUSED) == GST_STATE_CHANGE_FAILURE) {
            qCWarning(__gst__) << "[ GStreamer ] Unable to pause -" << m_request.url().toString();
            m_pendingState = m_state = MultimediaPlayer::StoppedState;
            emit stateChanged(m_state);
        } else {
            resumeVideoProbes();
            return true;
        }
    }

    return false;
}

void GstreamerPlayerEngine::stop()
{
    qCInfo(__gst__) << "[ Stop ] when" << m_state;

    m_everPlayed = false;
    if (m_playbin) {

        if (m_renderer)
            m_renderer->stopRenderer();

        flushVideoProbes();
        gst_element_set_state(m_playbin, GST_STATE_NULL);

        m_lastPosition = 0;
        MultimediaPlayer::State oldState = m_state;
        m_pendingState = m_state = MultimediaPlayer::StoppedState;

        finishVideoOutputChange();

        //we have to do it here, since gstreamer will not emit bus messages any more
        setSeekable(false);
        if (oldState != m_state)
            emit stateChanged(m_state);
    }
}

bool GstreamerPlayerEngine::seek(qint64 ms)
{
    //seek locks when the video output sink is changing and pad is blocked
    if (m_playbin && !m_pendingVideoSink && m_state != MultimediaPlayer::StoppedState && m_seekable) {
        qCInfo(__gst__, "[ seek ] = %lld ms", ms);

        ms = qMax(ms, qint64(0));
        gint64  position = ms * 1000000;
        bool isSeeking = gst_element_seek(m_playbin,
                                          m_playbackRate,
                                          GST_FORMAT_TIME,
                                          GstSeekFlags(GST_SEEK_FLAG_FLUSH),
                                          GST_SEEK_TYPE_SET,
                                          position,
                                          GST_SEEK_TYPE_NONE,
                                          0);
        if (isSeeking)
            m_lastPosition = ms;

        return isSeeking;
    }else{
        if(m_state == MultimediaPlayer::StoppedState)
            qCWarning(__gst__) << "[ seek ] when" << m_state;
        if(!m_seekable)
            qCWarning(__gst__) << "[ seek ] when seekable =" << m_seekable;
    }

    return false;
}

void GstreamerPlayerEngine::setVolume(int volume)
{
    if (m_volume != volume) {
        qCInfo(__gst__, "[ SetVolume ] = %d", volume);

        m_volume = volume;

        if (m_volumeElement)
            g_object_set(G_OBJECT(m_volumeElement), "volume", m_volume / 100.0, NULL);

        emit volumeChanged(m_volume);
    }
}

void GstreamerPlayerEngine::setMuted(bool muted)
{
    if (m_muted != muted) {
        qCInfo(__gst__, "[ SetMuted ] = %s", muted ? "true" : "false");

        m_muted = muted;

        if (m_volumeElement)
            g_object_set(G_OBJECT(m_volumeElement), "mute", m_muted ? TRUE : FALSE, NULL);

        emit mutedStateChanged(m_muted);
    }
}

void GstreamerPlayerEngine::showPrerollFrames(bool enabled)
{
    qCInfo(__gst__, "[ ShowPrerollFrames ] = %s", enabled ? "true" : "false");

    if (enabled != m_displayPrerolledFrame && m_videoSink &&
            g_object_class_find_property(G_OBJECT_GET_CLASS(m_videoSink), "show-preroll-frame") != 0) {

        gboolean value = enabled;
        g_object_set(G_OBJECT(m_videoSink), "show-preroll-frame", value, NULL);
        m_displayPrerolledFrame = enabled;
    }
}

// PRIVATE SLOTS
void GstreamerPlayerEngine::setSeekable(bool seekable)
{
    if (seekable != m_seekable) {
        qCInfo(__gst__, "[ SetSeekable ] = %s", seekable ? "true" : "false");

        m_seekable = seekable;
        emit seekableChanged(m_seekable);
    }
}

void GstreamerPlayerEngine::finishVideoOutputChange()
{
    if (!m_pendingVideoSink)
        return;

    qCInfo(__gst__) << "finishVideoOutputChange" << m_pendingVideoSink;
}

void GstreamerPlayerEngine::updateVideoRenderer()
{
    qCInfo(__gst__) << "Video sink has chaged, reload video output";

    if (m_videoOutput)
        setVideoRenderer(m_videoOutput);
}
#include <QThread>
void GstreamerPlayerEngine::updateDuration()
{
    gint64 gstDuration = 0;
    qint64 duration = -1;

    if (m_playbin && hs_gst_element_query_duration(m_playbin, GST_FORMAT_TIME, &gstDuration))
        duration = gstDuration / 1000000;

    if (m_duration != duration) {
        qCInfo(__gst__, "[ UpdateDuration ] = %lld ms", duration);

        m_duration = duration;
        emit durationChanged(m_duration);
    }

    gboolean seekable = false;
    if (m_duration > 0) {
        m_durationQueries = 0;
        GstQuery *query = gst_query_new_seeking(GST_FORMAT_TIME);
        if (gst_element_query(m_playbin, query))
            gst_query_parse_seeking(query, 0, &seekable, 0, 0);
        gst_query_unref(query);
    }
    setSeekable(seekable);

    if (m_durationQueries > 0) {
        qCInfo(__gst__, "[ UpdateDuration ] Queries = %d", m_durationQueries);

        //increase delay between duration requests
        int delay = 25 << (5 - m_durationQueries);
        m_durationQueries--;
//        QTimer::singleShot(delay, this, SLOT(updateDuration()));
        QThread::msleep(delay);
        updateDuration();
    }
}

// PRIVATE
void GstreamerPlayerEngine::handleVolumeChange(GObject *o, GParamSpec *p, gpointer d)
{
    Q_UNUSED(o);
    Q_UNUSED(p);
    GstreamerPlayerEngine *engine = reinterpret_cast<GstreamerPlayerEngine *>(d);
    QMetaObject::invokeMethod(engine, "updateVolume", Qt::QueuedConnection);
}

void GstreamerPlayerEngine::updateVolume()
{
    double volume = 1.0;
    g_object_get(m_playbin, "volume", &volume, NULL);

    if (m_volume != int(volume*100 + 0.5)) {
        m_volume = int(volume*100 + 0.5);

        qCInfo(__gst__, "[ UpdateVolume ] = %d", m_volume);

        emit volumeChanged(m_volume);
    }
}

void GstreamerPlayerEngine::handleMutedChange(GObject *o, GParamSpec *p, gpointer d)
{
    Q_UNUSED(o);
    Q_UNUSED(p);
    GstreamerPlayerEngine *engine = reinterpret_cast<GstreamerPlayerEngine *>(d);
    QMetaObject::invokeMethod(engine, "updateMuted", Qt::QueuedConnection);
}

void GstreamerPlayerEngine::updateMuted()
{
    gboolean muted = FALSE;
    g_object_get(G_OBJECT(m_playbin), "mute", &muted, NULL);
    if (m_muted != muted) {
        qCInfo(__gst__, "[ UpdateMuted ] = %s", m_muted ? "true" : "false");

        m_muted = muted;
        emit mutedStateChanged(muted);
    }
}

//doing proper operations when detecting an invalidMedia: change media status before signal the erorr
void GstreamerPlayerEngine::processInvalidMedia(MultimediaPlayer::Error errorCode, const QString& errorString)
{
    qCInfo(__gst__) <<  "[ ProcessInvalidMedia ] " << errorCode << errorString;

    emit invalidMedia();
    stop();
    emit error(int(errorCode), errorString);
}

void GstreamerPlayerEngine::removeVideoBufferProbe()
{
//    if (!m_videoProbe)
//        return;

//    GstPad *pad = gst_element_get_static_pad(m_videoSink, "sink");
//    if (pad) {
//        m_videoProbe->removeProbeFromPad(pad);
//        gst_object_unref(GST_OBJECT(pad));
//    }
}

void GstreamerPlayerEngine::addVideoBufferProbe()
{
//    if (!m_videoProbe)
//        return;

//    GstPad *pad = gst_element_get_static_pad(m_videoSink, "sink");
//    if (pad) {
//        m_videoProbe->addProbeToPad(pad);
//        gst_object_unref(GST_OBJECT(pad));
//    }
}

void GstreamerPlayerEngine::flushVideoProbes()
{
//    if (m_videoProbe)
//        m_videoProbe->startFlushing();
}

void GstreamerPlayerEngine::resumeVideoProbes()
{
//    if (m_videoProbe)
//        m_videoProbe->stopFlushing();
}



HS_END_NAMESPACE
