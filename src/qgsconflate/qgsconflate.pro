QGIS_DIR = /opt/Quantum-GIS
OGR_API_DIR = /usr/include/gdal
#GEOC_DIR = /opt/geoc_bez_idx
GEOC_DIR = /opt/geoc_cpp
QGIS_PLUGIN_DIR = /opt/qgis_plugins

TEMPLATE = lib

CONFIG += qt

QT += xml

unix:LIBS += -L/usr/lib \
    -L$$QGIS_DIR/build/output/lib \
    -L$$QGIS_DIR/build/output/lib/qgis/plugins/ \
    -L$$QGIS_PLUGIN_DIR \
    -L$$GEOC_DIR \
    -lqgis_core \
    -lqgis_gui \
    -lproj \
    -logrprovider \
    -lgeoc

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

DEST = qgsconflateplugin.so

DEFINES += GUI_EXPORT= \
    CORE_EXPORT=

HEADERS += \
    dialog.h \
    qgsconflateplugin.h \
    qgsconflateprovider.h

SOURCES += \
    dialog.cpp \
    qgsconflateplugin.cpp \
    qgsconflateprovider.cpp

FORMS += \
    dialog.ui

RESOURCES += \
    resources.qrc
