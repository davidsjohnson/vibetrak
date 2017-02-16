include(../defaults.pri)

TARGET = MalletTracker

TEMPLATE = app
CONFIG += app_bundle

SOURCES += \
    main.cpp

LIBS += -L../src -lvibetrak
