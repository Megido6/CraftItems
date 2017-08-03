#-------------------------------------------------
#
# Project created by QtCreator 2016-12-15T01:16:40
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = CraftItems
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp

HEADERS  += mainwindow.h

FORMS    += mainwindow.ui
win32:RC_ICONS = icon.ico

