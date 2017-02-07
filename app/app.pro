include(../defaults.pri)

TARGET = VibeTrak

TEMPLATE = app
#CONFIG += console
CONFIG += app_bundle

SOURCES += \
    main.cpp

LIBS += -L../src -lvibetrak
