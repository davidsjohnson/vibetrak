QT += core
QT -= gui

CONFIG += c++11

TARGET = VPT_Viewer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    videostream.cpp \
    webcamstream.cpp \
    onistream.cpp \
    colordetector.cpp

HEADERS += \
    videostream.h \
    webcamstream.h \
    onistream.h \
    utils.h \
    colordetector.h



 LIBS += -L/usr/local/Cellar/opencv3/HEAD-6328076_4/lib/ \
            -lopencv_core \
            -lopencv_videoio \
            -lopencv_highgui \
            -lopencv_imgproc

INCLUDEPATH += /usr/local/Cellar/opencv3/HEAD-6328076_4/include
DEPENDPATH +=  /usr/local/Cellar/opencv3/HEAD-6328076_4/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../OpenNI-MacOSX-x64-2.2/Redist/release/ -lOpenNI2
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../OpenNI-MacOSX-x64-2.2/Redist/debug/ -lOpenNI2
else:unix: LIBS += -L$$PWD/../../OpenNI-MacOSX-x64-2.2/Redist/ -lOpenNI2

INCLUDEPATH += $$PWD/../../OpenNI-MacOSX-x64-2.2/include
DEPENDPATH += $$PWD/../../OpenNI-MacOSX-x64-2.2/include
