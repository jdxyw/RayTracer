#-------------------------------------------------
#
# Project created by QtCreator 2011-04-01T10:19:46
#
#-------------------------------------------------

QT       += core gui

TARGET = RayTracer
TEMPLATE = app

INCLUDEPATH +=   C:\OpenCV2.2\include
INCLUDEPATH +=   C:\OpenCV2.2\include\opencv

LIBS +=   C:\OpenCV2.2\lib\*


SOURCES += main.cpp\
        mainwindow.cpp \
    scene.cpp \
    surface.cpp \
    raytracer.cpp

HEADERS  += mainwindow.h \
    common.h \
    scene.h \
    raytracer.h \
    surface.h

FORMS    += mainwindow.ui
