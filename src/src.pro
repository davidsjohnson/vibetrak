include(../defaults.pri)

TARGET = vibetrak
TEMPLATE = lib

SOURCES += onistream.cpp streamwidget.cpp webcamstream.cpp vibeframe.cpp colordetector.cpp \
   vibeosc.cpp \
    sendmalletcoords.cpp \
    streamrecorder.cpp \
    annotationwidget.cpp \
    utils.cpp

HEADERS += onistream.h streamwidget.h webcamstream.h utils.h videostream.h vibeframe.h colordetector.h \
   vibeosc.h \
    frameprocessor.h \
    sendmalletcoords.h \
    streamrecorder.h \
    annotationwidget.h
