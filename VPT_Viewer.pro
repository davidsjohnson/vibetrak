QT += core
QT -= gui

CONFIG += c++11

TARGET = VPT_Viewer
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    videostream.cpp

HEADERS += \
    videostream.h
