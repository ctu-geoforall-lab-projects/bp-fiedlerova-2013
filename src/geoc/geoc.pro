#-------------------------------------------------
#
# Project created by QtCreator 2013-02-03T09:54:46
#
#-------------------------------------------------

QT       -= core gui

TARGET = geoc
TEMPLATE = lib

DEFINES += GEOC_LIBRARY

SOURCES += geoc.cpp \
    vertexsnapper.cpp

HEADERS += geoc.h\
        geoc_global.h \
    vertexsnapper.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE9712CA3
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = geoc.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}
