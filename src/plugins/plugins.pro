
TEMPLATE = subdirs


qtHaveModule(quick) {
}

unix:!mac:!android {
    SUBDIRS += gstreamer\
               nativeplaylist
}
