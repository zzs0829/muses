TEMPLATE = subdirs

SUBDIRS += multimedia
SUBDIRS += src_plugins

src_plugins.subdir = plugins
src_plugins.depends = multimedia

qtHaveModule(quick) {
    SUBDIRS += imports
    imports.depends += multimedia
}
