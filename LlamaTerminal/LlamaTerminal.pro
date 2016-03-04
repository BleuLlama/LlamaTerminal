#-------------------------------------------------
#
# Project created by QtCreator 2016-02-24T17:08:13
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LlamaTerminal
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    palettedframebuffer.cpp \
    serialinterface.cpp \
    textbuffer.cpp \
    font.cpp

HEADERS  += mainwindow.h \
    palettedframebuffer.h \
    serialinterface.h \
    textbuffer.h \
    font.h

FORMS    += mainwindow.ui
