#-------------------------------------------------
#
# Project created by QtCreator 2015-10-29T01:47:26
#
#-------------------------------------------------

QT       += core gui multimedia printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = FindMath6
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    background.cpp \
    plugin.cpp \
    buttonwidget.cpp \
    penwidthlabel.cpp \
    element.cpp \
    mathline.cpp \
    drawlabel.cpp \
    calculate.cpp \
    scribblearea.cpp \
    advancedwidget.cpp \
    stringlabel.cpp

HEADERS  += mainwindow.h \
    background.h \
    plugin.h \
    buttonwidget.h \
    penwidthlabel.h \
    element.h \
    mathline.h \
    drawlabel.h \
    calculate.h \
    scribblearea.h \
    advancedwidget.h \
    stringlabel.h

INCLUDEPATH+=   D:\ProgramFiles\Tesseract-OCR\include\
                D:\ProgramFiles\Tesseract-OCR\include\leptonica\
                D:\ProgramFiles\Tesseract-OCR\include\tesseract

LIBS+=D:\ProgramFiles\Tesseract-OCR\lib\liblept168.lib
LIBS+=D:\ProgramFiles\Tesseract-OCR\lib\liblept168d.lib
LIBS+=D:\ProgramFiles\Tesseract-OCR\lib\libtesseract302.lib
LIBS+=D:\ProgramFiles\Tesseract-OCR\lib\libtesseract302d.lib

INCLUDEPATH+=   D:\ProgramFiles\opencv\build\include\
                D:\ProgramFiles\opencv\build\include\opencv\
                D:\ProgramFiles\opencv\build\include\opencv2

LIBS+=D:\ProgramFiles\opencv\build\x86\vc12\lib\opencv_ts300d.lib
LIBS+=D:\ProgramFiles\opencv\build\x86\vc12\lib\opencv_world300d.lib

DISTFILES += \
    ButtonPhoto/canvas mode.png \
    ButtonPhoto/general mode.png

RESOURCES += \
    myresources.qrc
