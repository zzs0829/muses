#include "multimediaresourcepolicy.h"
#include "multimediaresourcesetinterface.h"

HS_BEGIN_NAMESPACE
class DefaultMediaPlayerResourceSet : public MultimediaResourceSetInterface
{
public:
    DefaultMediaPlayerResourceSet(QObject *parent)
        : MultimediaResourceSetInterface(parent),
          m_granted(false),
          m_available(false),
          m_videoEnabled(false)
    {
    }

    bool isVideoEnabled() const Q_DECL_OVERRIDE
    {
        return m_videoEnabled;
    }

    bool isGranted() const Q_DECL_OVERRIDE
    {
        return m_granted;
    }

    bool isAvailable() const Q_DECL_OVERRIDE
    {
        return m_available;
    }

    void acquire() Q_DECL_OVERRIDE {
        if(!m_granted) {
            m_granted = true;
            emit resourcesGranted();
        }
    }
    void release() Q_DECL_OVERRIDE {
        if(m_granted) {
            m_granted = false;
            emit resourcesReleased();
            emit resourcesLost();
        }
    }
    void setVideoEnabled(bool enabled) Q_DECL_OVERRIDE {
        if(enabled != m_videoEnabled) {
            m_videoEnabled = enabled;
            if(m_videoEnabled) {
                emit resourcesGranted();
            }else{
                emit resourcesLost();
            }
        }

    }
private:
    bool m_granted;
    bool m_available;
    bool m_videoEnabled;
};

Q_GLOBAL_STATIC(QObject, defaultRoot)

QObject* MultimediaResourcePolicy::createResourceSet(const QString& interfaceId)
{
    QObject* obj = 0;
    if (!obj) {
        if (interfaceId == QLatin1String(MultimediaResourceSetInterface_iid)) {
            obj = new DefaultMediaPlayerResourceSet(defaultRoot());
        }
    }
    Q_ASSERT(obj);
    return obj;
}

void MultimediaResourcePolicy::destroyResourceSet(QObject* resourceSet)
{
    if (resourceSet->parent() == defaultRoot()) {
        delete resourceSet;
        return;
    }
}

HS_END_NAMESPACE
