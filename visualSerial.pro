#-------------------------------------------------
#
# Project created by QtCreator 2020-12-04T17:46:58
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = visualSerial
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    lcdcomm.cpp

HEADERS  += mainwindow.h \
    lcdcomm.h \
    visualconstanst.h

FORMS    += mainwindow.ui
