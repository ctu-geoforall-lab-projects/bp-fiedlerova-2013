/***************************************************************************
    qgsconflateprovider.cpp

    QgsConflateProvider
    functionality for plugin Conflate

    ---------------------
    begin                : April 2013
    copyright            : (C) 2013 by Tereza Fiedlerová
    email                : tfiedlerova dot at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/


// local includes
#include "qgsconflateprovider.h"

// QGIS includes
#include <qgsogrprovider.h>
#include <qgsvectorfilewriter.h>
#include <qgsvectorlayerfeatureiterator.h>
#include <qgsproject.h>
#include <qgsgeometry.h>

// Qt includes
#include <QTranslator>


QgsConflateProvider::QgsConflateProvider()
{
    // initialize data
    mRefLayer = NULL;
    mSubLayer = NULL;
    mNewLayer = NULL;

    mTolDistance = 0;
    correct = false;

} // constructor


QgsConflateProvider::~QgsConflateProvider()
{

} // destructor


bool QgsConflateProvider::createEmptyLayer( QString uri )
{

    // new vector layer as copy of mSubLayer
    QMap<int, int> *oldToNewAttrIdxMap = new QMap<int, int>;

    // parameters of new vector layer
    QString *errorMessage = NULL;
    QgsFields fields = mSubLayer->dataProvider()->fields();
    QGis::WkbType wkbType = mSubLayer->wkbType();
    QgsCoordinateReferenceSystem srs = (mSubLayer->crs());

    // options for new vector layer
    QMap <QString, QVariant> *options = new QMap<QString, QVariant>;
    options->insert("fileEncoding", "utf-8");
    options->insert("driverName", "ESRI Shapefile");

    // create empty layer
    QgsVectorLayerImport::ImportError ierror = QgsOgrProvider::createEmptyLayer(uri, fields, wkbType, &srs,
                                                true, oldToNewAttrIdxMap, errorMessage, options);

    // return false if error
    if( ierror )
    {
        QMessageBox::warning( 0, QTranslator::tr("Error"), QTranslator::tr("Error while creating layer"),
                              QMessageBox::Ok);

        delete options;
        delete oldToNewAttrIdxMap;

        return false;
    }

    qDebug( "QgsConflateProvider::createEmptyLayer: Empty shapefile created." );

    delete options;
    delete oldToNewAttrIdxMap;

    return true;

} // bool QgsConflateProvider::createEmptyLayer()


QString QgsConflateProvider::newUri( QString path, int &rank )
{
    int i = 1;
    QString source = path+"("+QString::number(i)+")";

    // find unexisting file
    while ( fileExists( source+".shp" ) )
    {
        i++;
        source = path+"("+QString::number(i)+")";

    }

    rank = i;
    return source+".shp";

} // QString QgsConflateProvider::newUri(QString path, QString name)


bool QgsConflateProvider::fileExists( QString source )
{

    QFile file(source);

    return file.exists();

} // bool QgsConflateProvider::fileExists(QString source)


bool QgsConflateProvider::copyLayer( QString uri )
{

    // stop if there is no subject layer
    if( mSubLayer == NULL )
    {
        qDebug( "QgsConflateProvider::copyLayer: No layer in layer tree." );
        return false;
    }

    // create empty layer
    int rank = 0;
    QString name;

    if ( uri.isEmpty())
    {
        uri = newUri( QgsProject::instance()->readPath( mSubLayer->name() ), rank );
        name = mSubLayer->name()+"("+QString::number(rank)+")";
    }
    // extract name
    else
    {
        if ( uri.rightRef(4) != ".shp" )
        {
                uri.append(".shp");
        }

        // extract name of layer from uri
        int uriSize = uri.size();
        for (int i = uriSize; i > 0; i--)
        {
            if ( uri[i] == QChar::fromAscii('/') )
            {
                name = uri.right( uriSize-i-1 );
                name.remove( name.size()-4, 4); // remove .shp from name
                break;
            }

        }

        if (name.isEmpty()) // for relative paths
        {
            name = uri;
            name.remove( name.size()-4, 4);
        }

    }

    if ( !createEmptyLayer( uri ) )
    {
        return false;
    }

    //new layer
    mNewLayer = new QgsVectorLayer(uri, name, mSubLayer->providerType(), "ogr");

    // copy features from subject layer to the new layer
    QgsFeature myFeature;
    QgsFeatureRequest request = QgsFeatureRequest(); // default feature request
    QgsVectorLayerFeatureIterator featureIt = QgsVectorLayerFeatureIterator(mSubLayer, request);
    featureIt.rewind(); // reset iterator to the start position
    QgsFeatureList features;

    while( featureIt.nextFeature( myFeature ) )
    {
        // add feature to the list
        features.push_back( myFeature );
    }


    // add features from list to the layer
    if( mNewLayer->dataProvider()->addFeatures(features) )
    {
        qDebug( "QgsConflateProvider::copyLayer: Features added." );
    }
    else
    {
        qDebug( "QgsConflateProvider::copyLayer: Unable to add features." );
    }

    // add layer if valid
    if( mNewLayer->isValid() )
    {
        qDebug( "QgsConflateProvider::copyLayer: Layer is valid." );
        return true;
    }
    // invalid layer
    else
    {
        qDebug( "QgsConflateProvider::copyLayer: Layer is NOT valid." );
    }

    return false;

} // bool QgsConflateProvider::copyLayer()


bool QgsConflateProvider::checkLayersType()
{
    QGis::GeometryType typeRef = mRefLayer->geometryType();
    QGis::GeometryType typeSub = mSubLayer->geometryType();

    if ( typeRef == QGis::UnknownGeometry || typeSub == QGis::UnknownGeometry
         || typeRef == QGis::NoGeometry || typeSub == QGis::NoGeometry)
    {
        return false;
    }

    return typeRef == typeSub;

} // bool QgsConflateProvider::checkLayersType()


void QgsConflateProvider::transferGeometrytoGeos( bool isRefLayer )
{

    QgsVectorLayer *myLayer;
    TGeomLayer *myGeosLayer;

    if ( isRefLayer )
    {
        myLayer = mRefLayer;
        myGeosLayer = &mGeosRef;
    }
    else
    {
        myLayer = mSubLayer;
        myGeosLayer = &mGeosSub;
    }

    myGeosLayer->clear();

    // transfer geometry of each feature from subject layer to geos
    QgsFeatureRequest request = QgsFeatureRequest(); // default feature request
    QgsVectorLayerFeatureIterator featureIt = QgsVectorLayerFeatureIterator( myLayer, request );
    featureIt.rewind(); // reset iterator to the start position

    QgsFeature myFeature;

    while( featureIt.nextFeature(myFeature) )
    {
        // geometry of the feature
        QgsGeometry *geom = myFeature.geometry();

        // transfer qgis geometry to geos
        GEOCGeom geos;
        geos.setFeatureId( myFeature.id() );
        geos.setGEOSGeomFromWKT( (geom->exportToWkt()).toStdString() );

        // add geometry to the list of geos geometries
        myGeosLayer->push_back(geos);

    }

    qDebug("QgsConflateProvider::transferGeometrytoGeos: GEOMETRY TRANSFERED TO GEOC.");

} // bool QgsConflateProvider::transferGeometryToGeos()


bool QgsConflateProvider::transferGeometryFromGeos()
{

    // some tests
    int caps = mNewLayer->dataProvider()->capabilities();

    if( !(caps & QgsVectorDataProvider::ChangeGeometries) )
    {
        return false;
    }

    // change geometry in the layer according to GEOS geometry
    QgsFeature myFeature;
    QgsFeatureRequest request = QgsFeatureRequest(); // default feature request
    QgsVectorLayerFeatureIterator featureIt = QgsVectorLayerFeatureIterator( mNewLayer, request );
    featureIt.rewind(); // reset iterator to the start position

    QgsGeometryMap geomMap;
    TGeomLayer::iterator it = mGeosNew.begin();

    QgsFeatureIds delFeatures;

    // next feature in the layer
    while ( featureIt.nextFeature( myFeature ) )
    {
        // get new geometry for the feature from geos geometry
        QgsGeometry *geom = NULL;
        int fid = myFeature.id();

        // check if fid are the same for both new and old geometry
        if( fid != (*it).getFeatureId() )
        {
            qDebug("QgsConflateProvider::transferGeometryFromGeos: different feature ids.");
            break;
        }

        // get new geometry if it is changed
        if ( (*it).isChanged() )
        {
            // delete feature with empty geometry
            if ( (*it).getGEOSGeom()->isEmpty() )
            {
                delFeatures.insert(fid);
            }

            // insert new geometry to the map of geometries
            QString wktGeom = QString::fromStdString((*it).getWKTGeom());
            geomMap.insert( fid, *(geom->fromWkt( wktGeom )) );

        }

        it++;
        delete geom;

    }

    // change geometries of features
    if( geomMap.size() > 0 && mNewLayer->dataProvider()->changeGeometryValues( geomMap) )
    {
        // delete required features
        if (delFeatures.size() > 0)
        {
           mNewLayer->dataProvider()->deleteFeatures( delFeatures );
        }

        qDebug("QgsConflateProvider::transferGeometryFromGeos: GEOMETRY TRANSFERED BACK");
        return true;
    }

    return false;


} // bool QgsConflateProvider::transferGeometryFromGeos()


void QgsConflateProvider::vertexSnap()
{

    // transfer ref and sub geometry
    bool isRef = true;
    transferGeometrytoGeos( isRef );
    transferGeometrytoGeos( !isRef );

    VertexSnapper vs = VertexSnapper();

    // set geometries of layers to vertex snapper
    vs.setRefGeometry( mGeosRef );
    vs.setSubGeometry( mGeosSub );

    // set tolerance distance and other
    vs.setTolDistance( mTolDistance );
    vs.setRepair(correct);

    // snap vertices from subject layer to the reference layer
    vs.snap();

    // set new geometry
    mGeosNew = vs.getNewGeometry();

    // get ids of invalid geometries and write them to the protocol
    vector<int> invalids = vs.getInvalidGeometries();
    writeProtocol(invalids, QTranslator::tr("Snap Vertices"));

    // transfer geometry back
    if ( mGeosNew.size() > 0 && transferGeometryFromGeos() )
    {
        qDebug("QgsConflateProvider::vertexSnap: SNAPPING DONE.");
    }

} // void QgsConflateProvider::vertexSnap()


void QgsConflateProvider::align()
{

    // transfer ref and sub geometry
    bool isRef = true;
    transferGeometrytoGeos( isRef );
    transferGeometrytoGeos( !isRef );

    // set geometries of layers for coverage alignment
    CoverageAlignment ca = CoverageAlignment();
    ca.setRefGeometry( mGeosRef );
    ca.setSubGeometry( mGeosSub );

    // set tolerance distance and other
    ca.setTolDistance( mTolDistance );
    ca.setRepair( correct );

    // align subject layer to reference layer
    ca.align();

    // set new geometry
    mGeosNew = ca.getNewGeometry();

    // get ids of invalid geometries and write them to the protocol
    vector<int> invalids = ca.getInvalidGeometries();
    writeProtocol(invalids, QTranslator::tr("Coverage Alignment"));

    // transfer geometry back
    if ( mGeosNew.size() > 0 && transferGeometryFromGeos() )
    {
        qDebug("QgsConflateProvider::align: ALIGNMENT DONE.");
    }

} // void QgsConflateProvider::align()


void QgsConflateProvider::lineMatch()
{

    if( mSubLayer->geometryType() != QGis::Line )
    {
        return;
    }

    // transfer ref and sub geometry
    bool isRef = true;
    transferGeometrytoGeos( isRef );
    transferGeometrytoGeos( !isRef );

    LineMatcher lm = LineMatcher();

    // set geometries of layers to line matcher
    lm.setRefGeometry( mGeosRef );
    lm.setSubGeometry( mGeosSub );

    // set tolerance distance and other
    lm.setTolDistance( mTolDistance );
    lm.setMatchTolerance( mMatchTol );

    // match lines of two layers
    lm.match();

    // set new geometry
    mGeosNew = lm.getNewGeometry();

    // get ids of invalid geometries and write them to the protocol
    vector<int> invalids = lm.getInvalidGeometries();
    writeProtocol(invalids, QTranslator::tr("Match Lines"));

    // transfer geometry back
    if ( mGeosNew.size() > 0 && transferGeometryFromGeos() )
    {
        qDebug("QgsConflateProvider::lineMatch: MATCHING DONE SUCESSFULY");
    }

} // void QgsConflateProvider::lineMatch()


void QgsConflateProvider::writeProtocol( const vector<int> &invalids, QString method )
{
    mProtocol.clear();

    // some general informations
    mProtocol = "Conflation protocol\n\nreference layer:\t\t"+mRefLayer->name()+
            "\nsubject layer:\t\t"+mSubLayer->name()+
            "\nnew layer:\t\t\t"+mNewLayer->name()+
            "\ndistance tolerance:\t\t"+QString::number(mTolDistance)+
            "\nconflation method:\t\t"+method+
            "\n\nnumber of processed features:\t"+QString::number(mGeosNew.size())+
            "\nnumber of invalid features:\t"+QString::number(invalids.size())+
            "\n\nids of invalid features (needs to be repaired manually):";


    // add list of invalid features
    for ( size_t i = 0; i < invalids.size(); i++ )
    {
        mProtocol.append( "\n"+QString::number(invalids[i]) );
    }

    if ( invalids.size() > 5)
    {
        mProtocol.append("\n\nNOTE: The high number of invalid features can be caused by "
                         "too large tolerance distance or by combining two layers with very "
                         "different precision. Please try conflation again with different "
                         "settings or check if data are topologically correct.");
    }

    mProtocol.append(QTranslator::tr("\n\nend of protocol"));

} // void QgsConflateProvider::writeProtocol( const vector<int> &invalids, QString method )
