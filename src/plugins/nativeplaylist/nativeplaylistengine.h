#ifndef NATIVEPLAYLISTENGINE_H
#define NATIVEPLAYLISTENGINE_H

#include <Multimedia/multimediaengine.h>

class NativePlaylistEngine : public MultimediaEngine
{
    Q_OBJECT
public:
    explicit NativePlaylistEngine(QObject *parent = 0);

    void setRootPath(const QString &path);

signals:

private:
    QString m_rootPath;
};

#endif // NATIVEPLAYLISTENGINE_H
