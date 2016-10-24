include(../defaults.pri)

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle

SOURCES += main.cpp

LIBS += -L../src -lvibetrak
