#-------------------------------------------------
#
# Project created by QtCreator 2015-02-06T09:29:18
#
#-------------------------------------------------

QT       += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = BET2
TEMPLATE = app


SOURCES += main.cpp\
        widget.cpp \
    glwidget.cpp \
    Algo.cpp \
    Deformation.cpp \
    DrawTriangle.cpp \
    fillData.cpp \
    VectOperators.cpp

HEADERS  += widget.h \
    glwidget.h \
    Algo.h \
    Deformation.h \
    DrawTriangle.h \
    fillData.h \
    VectOperators.h
