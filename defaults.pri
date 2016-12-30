INCLUDEPATH += $$PWD/src
SRC_DIR = $$PWD

QT += core
QT += widgets

CONFIG += c++14


LIBS += -L/usr/local/Cellar/opencv3/HEAD-acaa1e1_4/lib/ \
            -lopencv_core  \
            -lopencv_videoio \
            -lopencv_imgproc \
            -lopencv_highgui

INCLUDEPATH += /usr/local/Cellar/opencv3/HEAD-acaa1e1_4/include
DEPENDPATH +=  /usr/local/Cellar/opencv3/HEAD-acaa1e1_4/include

LIBS += -L/usr/local/OpenNI_2.2/Redist -lOpenNI2

INCLUDEPATH += /usr/local/OpenNI_2.2/include
DEPENDPATH  += /usr/local/OpenNI_2.2/include


INCLUDEPATH += /Users/fortjay81/oscpack_1_1_0
LIBS += -L/Users/fortjay81/oscpack_1_1_0 -loscpack


DISTFILES += \
    $$PWD/README.md

HEADERS +=

SOURCES +=
