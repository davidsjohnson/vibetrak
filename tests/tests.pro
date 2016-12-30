include(../defaults.pri)

TEMPLATE = app
CONFIG   += console
CONFIG   -= app_bundle

SOURCES += main.cpp

INCLUDEPATH+=/usr/local/include

LIBS += -L../src -lvibetrak

