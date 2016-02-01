QT       += core gui
QT       += network

CONFIG += c++11

TARGET = dps_server

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
QMAKE_CXXFLAGS += -std=c++0x

CONFIG += console
CONFIG += app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    mythread.cpp \
    myserver.cpp

HEADERS += \
    mythread.h \
    myserver.h

FORMS +=
