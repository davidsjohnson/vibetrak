TEMPLATE = subdirs
CONFIG+=ordered
SUBDIRS = \
    src \
    app  \
    gui \
    tests

app.depends = src
gui.depends = src
tests.depends = src
