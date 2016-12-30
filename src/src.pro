include(../defaults.pri)

TARGET = vibetrak
TEMPLATE = lib

SOURCES += onistream.cpp streamwidget.cpp webcamstream.cpp vibeframe.cpp colordetector.cpp \
   vibeosc.cpp

HEADERS += onistream.h streamwidget.h webcamstream.h utils.h videostream.h vibeframe.h colordetector.h \
   vibeosc.h
