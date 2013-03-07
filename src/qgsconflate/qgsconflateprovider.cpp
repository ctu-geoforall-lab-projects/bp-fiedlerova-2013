// local includes
#include "qgsconflateprovider.h"

// QGis includes
#include <qgsvectorlayer.h>
#include <qgsogrprovider.h>
#include <qgsvectorfilewriter.h>
#include <qgsvectorlayerfeatureiterator.h>
#include <qgsproject.h>
#include <qgsgeometry.h>

QgsConflateProvider::QgsConflateProvider()
{
    // initialize data
    mRefLayer = NULL;
    mSubLayer = NULL;
    mNewLayer = NULL;

    mGeosSub.clear();
    mGeosRef.clear();

    tolDistance = 0;

}

/*QgsConflateProvider::~QgsConflateProvider()
{
    if(mRefLayer) delete mRefLayer;
    if(mSubLayer) delete mSubLayer;
    if(mNewLayer) delete mNewLayer;
}*/

bool QgsConflateProvider::createEmptyLayer( QString uri )
{

    // new vector layer as copy of mSubLayer
    QMap<int, int> *oldToNewAttrIdxMap = new QMap<int, int>;

    // parameters of new vector layer
    QString *errorMessage = NULL;
    QgsFields fields = mSubLayer->pendingFields(); //mSubLayer->dataProvider()->fields();
    QGis::WkbType wkbType = mSubLayer->wkbType();
    QgsCoordinateReferenceSystem srs = (mSubLayer->crs());

    // options for new vector layer
    QMap <QString, QVariant> *options = new QMap<QString, QVariant>;
    options->insert("fileEncoding", "utf-8");
    options->insert("driverName", "ESRI Shapefile");

    // create empty layer
    QgsVectorLayerImport::ImportError ierror = QgsOgrProvider::createEmptyLayer(uri, fields, wkbType, &srs,
                                                true, oldToNewAttrIdxMap, errorMessage, options); // some memory error here

    // return false if error
    if( ierror )
    {
        QMessageBox::warning(0,"Error", "Error while creating layer", QMessageBox::Ok);
        delete options;
        delete oldToNewAttrIdxMap;
        return false;
    }

    qDebug( "QgsConflateProvider::createEmptyLayer: Empty vector layer as .shp created." );
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
    if (uri.isEmpty())
        uri = newUri( QgsProject::instance()->readPath( mSubLayer->name() ), rank ); //QgsProject::instance()->homePath(), rank);//mSubLayer->source()+"_copy.shp";

    if ( !createEmptyLayer( uri ) )
    {
        return false;
    }

    //new layer
    mNewLayer = new QgsVectorLayer(uri,
                                   mSubLayer->name()+"("+QString::number(rank)+")", mSubLayer->providerType(), "ogr");
    qDebug( "QgsConflateProvider::copyLayer: New vector layer created." );


    // copy features from subject layer to the new layer
    QgsFeature myFeature;
    QgsFeatureRequest request = QgsFeatureRequest(); // default feature request
    QgsVectorLayerFeatureIterator featureIt = QgsVectorLayerFeatureIterator(mSubLayer, request); // feature iterator
    featureIt.rewind(); // reset iterator to the start position
    QList<QgsFeature> features;

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

void QgsConflateProvider::transferGeometrytoGeos( bool isRefLayer )
{
    qDebug("QgsConflateProvider::transferGeometrytoGeos: ENTERING");

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
    QgsVectorLayerFeatureIterator featureIt = QgsVectorLayerFeatureIterator( myLayer, request ); // feature iterator
    featureIt.rewind(); // reset iterator to the start position

    QgsFeature myFeature;

    while( featureIt.nextFeature(myFeature) )
    {
        // geometry of the feature
        QgsGeometry *geom = myFeature.geometry();

        // transfer qgis geometry to geos
        MyGEOSGeom geos;
        geos.setFeatureId( myFeature.id() );
        geos.setGEOSGeom( GEOSGeom_clone(geom->asGeos()) ); // some memory error here

        // add geometry to the list of geos geometries
        myGeosLayer->push_back(geos);

    }

    qDebug("QgsConflateProvider::transferGeometrytoGeos: GEOMETRY TRANSFERED");

} // bool QgsConflateProvider::transferGeometryToGeos()


bool QgsConflateProvider::transferGeometryFromGeos()
{

    // AFTER DOING SOMETHING WITH GEOMETRY IN GEOS FORMAT

    // some tests
    int caps = mNewLayer->dataProvider()->capabilities();

    if( !(caps & QgsVectorDataProvider::ChangeGeometries) )
    {
        return false;
    }

    // change geometry in the layer according to GEOS geometry
    QgsFeature myFeature;
    QgsFeatureRequest request = QgsFeatureRequest(); // default feature request
    QgsVectorLayerFeatureIterator featureIt = QgsVectorLayerFeatureIterator( mNewLayer, request ); // feature iterator
    featureIt.rewind(); // reset iterator to the start position

    QgsGeometryMap geomMap;
    TGeomLayer::iterator it = mGeosSub.begin(); // geometry iterator

    // next feature in the layer
    while ( featureIt.nextFeature( myFeature ) )
    {
        // get new geometry for the feature from geos geometry
        QgsGeometry *geom = new QgsGeometry();
        GEOSGeometry * newGeom = GEOSGeom_clone( (*it).getGEOSGeom() );
        geom->fromGeos( newGeom );

        // insert new geometry to the map of new geometries
        geomMap.insert( myFeature.id(), *geom );//(*it).getFeatureId(), geom );
        it++;

        delete geom;

    }

    if( mNewLayer->dataProvider()->changeGeometryValues( geomMap ) )
    {
        qDebug("QgsConflateProvider::transferGeometryFromGeos: GEOMETRY TRANSFERED BACK");
        return true;
    }

    return false;


} // bool QgsConflateProvider::transferGeometryFromGeos()


void QgsConflateProvider::vertexSnap()
{
    //initGEOS(); ???
    transferGeometrytoGeos( true );
    transferGeometrytoGeos( false );
    qDebug("QgsConflateProvider::vertexSnap: GEOMETRY TRANSFERED IN VS");

    // DO SOMETHING WITH GEOMETRY IN GEOS FORMAT

    VertexSnapper vs = VertexSnapper();
    qDebug("QgsConflateProvider::vertexSnap: VERTEX SNAPPER CREATED");

    // set geometries of layers to vertex snapper
    vs.setRefGeometry( mGeosRef );
    vs.setSubGeometry( mGeosSub );

    // set tolerance distance
    vs.setTolDistance( tolDistance );

    // snap vertices from subject layer to the reference layer
    vs.snap();
    qDebug("QgsConflateProvider::vertexSnap: SNAP DONE");

    // set new geometry
    mGeosSub = vs.getNewGeometry();
    qDebug("QgsConflateProvider::vertexSnap: SET NEW GEOMETRY DONE");

    // transfer geometry back
    if ( transferGeometryFromGeos() )
    {
        qDebug("QgsConflateProvider::vertexSnap: SNAPPING DONE");
    }

    //finishGEOS(); ???

} // void QgsConflateProvider::vertexSnap()
