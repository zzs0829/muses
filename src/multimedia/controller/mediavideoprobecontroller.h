#ifndef MEDIAVIDEOPROBECONTROLLER_H
#define MEDIAVIDEOPROBECONTROLLER_H

#include <Multimedia/multimediacontroller.h>

HS_BEGIN_NAMESPACE

class VideoFrame;
class MediaVideoProbeController : public MultimediaController
{
    Q_OBJECT
public:
    virtual ~MediaVideoProbeController();

signals:
    void videoFrameProbed(const VideoFrame &frame);
    void flush();

protected:
    explicit MediaVideoProbeController(QObject *parent = Q_NULLPTR);
};

HS_END_NAMESPACE

#endif // MEDIAVIDEOPROBECONTROLLER_H
