#-------------------------------------------------
#
# Project created by QtCreator 2015-09-29T20:14:25
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = jfc_debugger
TEMPLATE = app

QTPLUGIN += qjpeg

SOURCES += main.cpp\
        mainwindow.cpp \
    xlogic.cpp \
    xnetsock.cpp \
    xparser.cpp \
    nobtndialog.cpp

HEADERS  += mainwindow.h \
    xlogic.h \
    xnetsock.h \
    xparser.h \
    nobtndialog.h \
    defines.h

FORMS    += mainwindow.ui \
    nobtndialog.ui

RESOURCES +=

DISTFILES +=
