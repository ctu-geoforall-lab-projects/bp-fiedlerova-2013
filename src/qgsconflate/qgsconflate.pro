QGIS_DIR = /home/tereza/QGIS/dev/cpp/Quantum-GIS

TEMPLATE = lib

CONFIG = qt

QT += xml

unix:LIBS += -L/$$QGIS_DIR/lib \
    -lqgis_core \
    -lqgis_gui

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
    $$QGIS_DIR/build/src/ui \
    $$QGIS_DIR

DEST = qgsconflateplugin.so

DEFINES += GUI_EXPORT= \
    CORE_EXPORT=

HEADERS += \
    dialog.h \
    qgsconflateplugin.h

SOURCES += \
    dialog.cpp \
    qgsconflateplugin.cpp

FORMS += \
    dialog.ui
