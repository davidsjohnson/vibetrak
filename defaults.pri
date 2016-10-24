INCLUDEPATH += $$PWD/src
SRC_DIR = $$PWD

QT += core
QT -= gui

CONFIG += c++11


LIBS += -L/usr/local/Cellar/opencv3/HEAD-6328076_4/lib/ \
            -lopencv_core \
            -lopencv_videoio \
            -lopencv_highgui \
            -lopencv_imgproc

INCLUDEPATH += /usr/local/Cellar/opencv3/HEAD-6328076_4/include
DEPENDPATH +=  /usr/local/Cellar/opencv3/HEAD-6328076_4/include

LIBS += -L/usr/local/OpenNI_2.2/Redist/ -lOpenNI2

INCLUDEPATH += /usr/local/OpenNI_2.2/include
DEPENDPATH  += /usr/local/OpenNI_2.2/include

INCLUDEPATH += /usr/local/boost_1_62_0
