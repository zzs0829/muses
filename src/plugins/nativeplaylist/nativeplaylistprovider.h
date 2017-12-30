#ifndef NATIVEPLAYLISTPROVIDER_H
#define NATIVEPLAYLISTPROVIDER_H

#include <Multimedia/mediaplaylistprovider.h>

class NativePlaylistProviderPrivate;
class NativePlaylistProvider : public MediaPlaylistProvider
{
    Q_OBJECT
public:
    NativePlaylistProvider(const QString &path,
                           const QStringList &nameFilters,
                           QObject *parent = 0);
    ~NativePlaylistProvider();

    bool load(const QNetworkRequest &request, const char *format = 0) Q_DECL_OVERRIDE;

    int mediaCount() const Q_DECL_OVERRIDE;
    MediaContent media(int pos) const Q_DECL_OVERRIDE;

    bool isReadOnly() const Q_DECL_OVERRIDE;

public Q_SLOTS:
    void shuffle() Q_DECL_OVERRIDE;

private:
    Q_DISABLE_COPY(NativePlaylistProvider)
    Q_DECLARE_PRIVATE(NativePlaylistProvider)
//    Q_PRIVATE_SLOT(d_func(), void _q_handleParserError(QPlaylistFileParser::ParserError err, const QString &))
    Q_PRIVATE_SLOT(d_func(), void _q_handleNewItem(const QList<MediaContent> &medias))

};

#endif // NATIVEPLAYLISTPROVIDER_H
