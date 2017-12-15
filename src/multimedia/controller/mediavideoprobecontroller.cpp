#include "mediavideoprobecontroller.h"
#include "../multimediacontroller_p.h"

HS_BEGIN_NAMESPACE

MediaVideoProbeController::MediaVideoProbeController(QObject *parent)  :
    MultimediaController(*new MultimediaControllerPrivate, parent)
{

}
MediaVideoProbeController::~MediaVideoProbeController()
{
}

HS_END_NAMESPACE
