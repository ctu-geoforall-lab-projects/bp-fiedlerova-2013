# -------------------------------------------------
# Project created by QtCreator 2013-02-03T09:54:46
# -------------------------------------------------
DEFINES += GEOCLIBRARY
TEMPLATE = lib
SOURCES += vertexsnapper.cpp \
    geoc_c.cpp
HEADERS += geoc.h \
    vertexsnapper.h \
    mygeosgeom.h \
    mygeometryeditoroperation.h \
    geoc_c.h
DEST = libgeoc.so
unix:LIBS += -L/usr/lib/ \
    -L/usr/local/lib \
    -lgeos \
    -lgeos_c
