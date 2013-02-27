#-------------------------------------------------
#
# Project created by QtCreator 2013-02-03T09:54:46
#
#-------------------------------------------------

DEFINES += GEOCLIBRARY

TEMPLATE = lib

SOURCES += \
    vertexsnapper.cpp

HEADERS += geoc.h\
    vertexsnapper.h \
    mygeosgeom.h \
    mygeometryeditoroperation.h


unix:LIBS += -L/usr/lib/ \
	-lgeos-3.3.3 \
	-lgeos_c

