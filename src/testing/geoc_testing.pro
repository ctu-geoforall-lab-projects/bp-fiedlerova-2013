#-------------------------------------------------
#
# Project created by QtCreator 2013-03-07T14:13:56
#
#-------------------------------------------------

QGIS_DIR = /opt/Quantum-GIS
OGR_API_DIR = /usr/include/gdal
GEOC_DIR = /opt/geoc_cpp
QGIS_PLUGIN_DIR = /opt/qgsconflate

TEMPLATE = app

CONFIG   += console
CONFIG   += qt
CONFIG   -= app_bundle

QT       += xml

TARGET = geoc_testing

unix:LIBS += -L/usr/lib \
    -L$$QGIS_DIR/build/output/lib \
    -L$$QGIS_DIR/build/output/lib/qgis/plugins/ \
    -L$$QGIS_PLUGIN_DIR \
    -L$$GEOC_DIR \
    -L$$QGIS_PLUGIN_DIR \
    -lqgis_core \
    -lqgis_gui \
    -lproj \
    -logrprovider \
    -lgeoc \
    -lgeos_c \
    -lqgsconflate \

INCLUDEPATH += $$QGIS_DIR/src/ui \
    $$QGIS_DIR/src/plugins \
    $$QGIS_DIR/src/gui \
    $$QGIS_DIR/src/raster \
    $$QGIS_DIR/src/core \
    $$QGIS_DIR/src/core/renderer \
    $$QGIS_DIR/src/core/symbology \
    $$QGIS_DIR/src/core/symbology-ng \
    $$QGIS_DIR/src/app \
    $$QGIS_DIR/src/app/legend \
    $$QGIS_DIR/src/ui \
    $$QGIS_DIR/build \
    $$QGIS_DIR/build/src/ui \
    $$QGIS_DIR/src/providers/ogr \
    $$QGIS_DIR/src/providers/gdal \
    $$QGIS_DIR \
    $$OGR_API_DIR \
    $$GEOC_DIR \
    $$QGIS_PLUGIN_DIR

DEFINES += GUI_EXPORT= \
    CORE_EXPORT=

SOURCES += main.cpp
