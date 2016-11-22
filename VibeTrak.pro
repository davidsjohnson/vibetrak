TEMPLATE = subdirs
CONFIG+=ordered
SUBDIRS = \
    src \
    app
#    tests \
#    gui
app.depends = src
#gui.depends = src
#tests.depends = src
