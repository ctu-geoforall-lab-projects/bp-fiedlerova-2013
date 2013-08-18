cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_BUILD_TYPE Release )
add_definitions ( -Wall )
add_definitions ( -DCORE_EXPORT= )
add_definitions ( -DGUI_EXPORT= )
find_package ( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )
set( QGIS_DIR /opt/Quantum-GIS )
set( GEOC_DIR /opt/geoc_cpp )
set( OGR_API_DIR  /usr/include/gdal )
set( QGIS_PLUGIN_DIR /opt/qgis_plugins )

include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR} ${QT_QTXML_INCLUDE_DIR}
	${QGIS_DIR}/src/plugins
	${QGIS_DIR}/src/gui
	${QGIS_DIR}/src/raster
	${QGIS_DIR}/src/core
	${QGIS_DIR}/src/core/renderer
	${QGIS_DIR}/src/core/symbology
	${QGIS_DIR}/src/core/symbology-ng
	${QGIS_DIR}/src/app
	${QGIS_DIR}/src/app/legend
	${QGIS_DIR}/src/ui
	${QGIS_DIR}/build
	${QGIS_DIR}/build/src/ui
	${QGIS_DIR}/src/providers/ogr
	${QGIS_DIR}/src/providers/gdal
	${QGIS_DIR}
	${OGR_API_DIR}
	${GEOC_DIR}
	${GEOC_DIR}/build
	${QGIS_PLUGIN_DIR})
	
set ( qgsconflate_HDRS
	qgsconflateprovider.h
	)

set ( qgsconflate_SRCS
	dialog.cpp
	qgsconflateplugin.cpp
	qgsconflateprovider.cpp
	)

set ( qgsconflate_UIS
	dialog.ui
	)
QT4_WRAP_UI(UIS ${qgsconflate_UIS})

set ( qgsconflate_RSCS
	resources.qrc
	)
QT4_ADD_RESOURCES(RSCS ${qgsconflate_RSCS})

set ( qgsconflate_MOCS
	dialog.h
	qgsconflateplugin.h
	)
QT4_WRAP_CPP(MOCS ${qgsconflate_MOCS})

add_library ( qgsconflate SHARED ${qgsconflate_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( qgsconflate  ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} ${QT_QTXML_LIBRARY} )
