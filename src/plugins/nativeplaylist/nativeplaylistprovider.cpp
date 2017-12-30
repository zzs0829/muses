#include "nativeplaylistprovider.h"
#include "nativeplaylistengine.h"
#include <QTime>
#include <QDebug>

class NativePlaylistProviderPrivate: public MediaPlaylistProviderPrivate
{
    Q_DECLARE_PUBLIC(NativePlaylistProvider)
public:
    bool load();

//    QPlaylistFileParser parser;
    NativePlaylistEngine *engine;
    QList<MediaContent> resources;

    void _q_handleNewItem(const QList<MediaContent> &medias);

    NativePlaylistProvider *q_ptr;
    QTime time;
};

bool NativePlaylistProviderPrivate::load()
{
    time.start();
    engine->startFindWork();

    return true;
}

void NativePlaylistProviderPrivate::_q_handleNewItem(const QList<MediaContent> &medias)
{
    resources = medias;
    qDebug() << "[ Found MediaContent ] " << resources.count() << "lines; " << time.elapsed() << "ms;";
}

NativePlaylistProvider::NativePlaylistProvider(const QString &path,
                                               const QStringList &nameFilters,
                                               QObject *parent) :
    MediaPlaylistProvider(*new NativePlaylistProviderPrivate, parent)
{
    d_func()->q_ptr = this;
    d_func()->engine = new NativePlaylistEngine(path, nameFilters, this);

    qRegisterMetaType<QList<MediaContent>>("QList<MediaContent>");
    connect(d_func()->engine, SIGNAL(mediaContentFound(QList<MediaContent>)),
            this, SLOT(_q_handleNewItem(QList<MediaContent>)));
    connect(d_func()->engine, SIGNAL(finished()), this, SIGNAL(loaded()));
//    connect(&d_func()->parser, SIGNAL(error(QPlaylistFileParser::ParserError,QString)),
//            this, SLOT(_q_handleParserError(QPlaylistFileParser::ParserError,QString)));

}

NativePlaylistProvider::~NativePlaylistProvider()
{

}

bool NativePlaylistProvider::isReadOnly() const
{
    return true;
}

bool NativePlaylistProvider::load(const QNetworkRequest &request, const char *format)
{
    Q_UNUSED(request)
    Q_UNUSED(format)

    return d_func()->load();
}

int NativePlaylistProvider::mediaCount() const
{
    return d_func()->resources.size();
}

MediaContent NativePlaylistProvider::media(int pos) const
{
    return d_func()->resources.value(pos);
}

void NativePlaylistProvider::shuffle()
{
    Q_D(NativePlaylistProvider);
    if (!d->resources.isEmpty()) {
        QList<MediaContent> resources;

        while (!d->resources.isEmpty()) {
            resources.append(d->resources.takeAt(qrand() % d->resources.size()));
        }

        d->resources = resources;
        emit mediaChanged(0, mediaCount()-1);
    }
}


#include "moc_nativeplaylistprovider.cpp"

