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
    featuresnapper.cpp

HEADERS += geoc.h\
    vertexsnapper.h \
    mygeosgeom.h \
    mygeometryeditoroperation.h \
    matchinggeometry.h \
    affinetransformation.h \
    triangulation.h \
    completeconflation.h \
    featuresnapper.h


unix:LIBS += -L/usr/local/lib \
        -lgeos-3.3.7 \
        -lgeos \
	-lgeos_c
