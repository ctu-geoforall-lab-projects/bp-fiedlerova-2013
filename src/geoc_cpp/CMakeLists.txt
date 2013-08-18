cmake_minimum_required( VERSION 2.6 )
set ( CMAKE_BUILD_TYPE Release )
add_definitions ( -Wall )
find_package ( Qt4 REQUIRED )
include ( ${QT_USE_FILE} )
include_directories (
	${CMAKE_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR}
	${QT_QTCORE_INCLUDE_DIR} ${QT_QTGUI_INCLUDE_DIR}
	)
set ( geoc_cpp_HDRS
	geoc.h
	vertexsnapper.h
	matchinggeometry.h
	affinetransformation.h
	triangulation.h
	vertexgeometryeditoroperation.h
	triangle.h
	coveragealignment.h
	aligngeometryeditoroperation.h
	sort.h
	geocgeom.h
	linematcher.h
	geometrycorrectionoperation.h
	spatialindexbuilder.h
	)

set ( geoc_cpp_SRCS
	vertexsnapper.cpp
	matchinggeometry.cpp
	affinetransformation.cpp
	triangulation.cpp
	coveragealignment.cpp
	aligngeometryeditoroperation.cpp
	linematcher.cpp
	geometrycorrectionoperation.cpp
	spatialindexbuilder.cpp
	vertexgeometryeditoroperation.cpp
	)

add_library ( geoc SHARED ${geoc_cpp_SRCS} ${UIS} ${RSCS} ${TRS} ${MOCS} )
target_link_libraries ( geoc  ${QT_QTCORE_LIBRARY} ${QT_QTGUI_LIBRARY} )
