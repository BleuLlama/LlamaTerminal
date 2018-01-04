#-------------------------------------------------
#
# Project created by QtCreator 2016-02-24T17:08:13
#
#
#    The MIT License (MIT)
#    Copyright (c) 2016 Scott Lawrence
#
#    Permission is hereby granted, free of charge, to any person
#    obtaining a copy of this software and associated documentation
#    files (the "Software"), to deal in the Software without
#    restriction, including without limitation the rights to use,
#    copy, modify, merge, publish, distribute, sublicense, and/or
#    sell copies of the Software, and to permit persons to whom the
#    Software is furnished to do so, subject to the following
#    conditions:
#
#    The above copyright notice and this permission notice shall be
#    included in all copies or substantial portions of the Software.
#
#    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
#    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
#    OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
#    NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
#    HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
#    WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
#    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
#    OTHER DEALINGS IN THE SOFTWARE.
#
#-------------------------------------------------

QT       += core gui serialport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = LlamaTerminal
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    palettedframebuffer.cpp \
    serialinterface.cpp \
    font.cpp \
    theme.cpp \
    textpipe.cpp \
    framedsprite.cpp

HEADERS  += mainwindow.h \
    palettedframebuffer.h \
    serialinterface.h \
    font.h \
    globals.h \
    theme.h \
    textpipe.h \
    framedsprite.h

FORMS    += mainwindow.ui
