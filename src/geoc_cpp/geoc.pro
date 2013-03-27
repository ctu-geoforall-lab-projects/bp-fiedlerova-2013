#-------------------------------------------------
#
# Project created by QtCreator 2013-02-03T09:54:46
#
#-------------------------------------------------

DEFINES += GEOCLIBRARY

TEMPLATE = lib

SOURCES += \
    vertexsnapper.cpp \
    matchinggeometry.cpp \
    affinetransformation.cpp \
    triangulation.cpp \
    completeconflation.cpp \
    featuresnapper.cpp \
    vertexgeometryoperation.cpp \
    conflategeometryeditoroperation.cpp \
    triangle.cpp

HEADERS += geoc.h\
    vertexsnapper.h \
    mygeosgeom.h \
    matchinggeometry.h \
    affinetransformation.h \
    triangulation.h \
    completeconflation.h \
    featuresnapper.h \
    vertexgeometryeditoroperation.h \
    conflategeometryeditoroperation.h \
    triangle.h


unix:LIBS += -L/usr/local/lib \
        -lgeos \
	-lgeos_c
