#include "mediaplayercontroller.h"
#include "../multimediacontroller_p.h"

HS_BEGIN_NAMESPACE

MediaPlayerController::MediaPlayerController(QObject *parent) :
    MultimediaController(*new MultimediaControllerPrivate, parent)
{

}

MediaPlayerController::~MediaPlayerController()
{
}

HS_END_NAMESPACE
