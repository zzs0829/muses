#include "gstreamerglobal.h"


// 环境变量开关：QT_LOGGING_RULES="muses.plugin.gstreamer.debug=false"
// USING LOG IN CPP
// Q_DECLARE_LOGGING_CATEGORY(__gst__)
Q_LOGGING_CATEGORY(__gst__, LOG_CATEGORY_GSTREAMER, QtMsgType::QtInfoMsg)
