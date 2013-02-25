all:
	g++ main.cpp -o geoc_testing -DCORE_EXPORT= -I/usr/include/qt4/QtCore -I/usr/include/qt4/QtGui \
	-I/usr/include/qt4 -I/opt/Quantum-GIS/src/core -I/usr/include/qt4/QtXml \
	-I/opt/Quantum-GIS/src/providers/ogr -I../qgsconflate \
	-L/opt/Quantum-GIS/build/output/lib/qgis/plugins \
	-L../qgsconflate \
	-lqgis_core -lQtCore -lQtGui -lQtXml -logrprovider -lqgsconflate

clean:
	rm -f *~ qgis.db geoc_testing