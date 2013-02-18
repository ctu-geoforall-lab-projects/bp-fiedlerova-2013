// Local includes
#include "dialog.h"

// Qt includes
#include <QMessageBox>

// QGis includes
#include <qgsmapcanvas.h>
#include <qgsvectorlayer.h>
#include <qgsmaplayerregistry.h>
#include <qgsogrprovider.h>
#include <qgsvectorfilewriter.h>
#include <qgsvectorlayerfeatureiterator.h>
#include <qgsproject.h>

// GEOS includes
#include <geos_c.h>


Dialog::Dialog(QWidget *parent, Qt::WFlags fl, QgisInterface * iface) :
    QDialog(parent, fl)
{
    setupUi(this);

    // set interface
    mIface = iface;

    // initialize data
    mRefLayer = NULL;
    mSubLayer = NULL;
    mNewLayer = NULL;

    // load map layers to the QMap container
    QMap<QString, QgsMapLayer*> mapLayers = QgsMapLayerRegistry::instance()->mapLayers();
    QMap<QString, QgsMapLayer*>::iterator layer_it = mapLayers.begin();

    //
    for ( ; layer_it != mapLayers.end(); ++layer_it )
    {
        // map layer as vector layer
        QgsVectorLayer* vl = dynamic_cast<QgsVectorLayer*>( layer_it.value() );

        // stop if it is not vector layer
        if ( !vl )
          continue;

        // add layer name to comboBox
        this->mcbReferences->insertItem( 0, vl->name() );
        this->mcbSubjects->insertItem( 0, vl->name() );
    }

    mGeosSub.clear();
    mGeosRef.clear();

} // constructor


Dialog::~Dialog()
{

}  // destructor


QgsVectorLayer* Dialog::selectedLayer(int index)
{

  // load map layers to the QMap container
  QMap<QString, QgsMapLayer*> mapLayers = QgsMapLayerRegistry::instance()->mapLayers();
  QMap<QString, QgsMapLayer*>::iterator layer_it = mapLayers.begin();

  // find the right layer
  for ( ; layer_it != mapLayers.end(); ++layer_it )
  {

    QgsVectorLayer* vl = dynamic_cast<QgsVectorLayer*>( layer_it.value() );

    if ( !vl )
      continue;

    // return layer according to selected name
    if ( (index == 0) && (vl->name() == mcbReferences->currentText()) )
      return vl;

    if( (index == 1) && (vl->name() == mcbSubjects->currentText()) )
        return vl;

  }

  // return NULL if there is no vector layer
  return NULL;

} // QgsVectorLayer* Dialog::selectedLayer(int index)


bool Dialog::createEmptyLayer( QString uri )
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
                                                false, oldToNewAttrIdxMap, errorMessage, options);

    // return false if error
    if( ierror )
    {
        QMessageBox::warning(0,"Error", "Error while creating layer", QMessageBox::Ok);
        return false;
    }

    qDebug( "Empty vector layer as .shp created." );
    return true;

} // bool Dialog::createEmptyLayer()


QString Dialog::newUri( QString path, int &rank )
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

} // QString Dialog::newUri(QString path, QString name)


bool Dialog::fileExists( QString source )
{

    QFile file(source);

    return file.exists();

} // bool Dialog::fileExists(QString source)


bool Dialog::copyLayer()
{

    // stop if there is no subject layer
    if( mSubLayer == NULL )
    {
        qDebug( "No layer in layer tree." );
        return false;
    }

    // create empty layer
    int rank = 0;
    QString uri = newUri( QgsProject::instance()->readPath( mSubLayer->name() ), rank ); //QgsProject::instance()->homePath(), rank);//mSubLayer->source()+"_copy.shp";

    if ( !createEmptyLayer( uri ) )
    {
        return false;
    }

    //new layer
    mNewLayer = new QgsVectorLayer(uri, mSubLayer->name()+"("+QString::number(rank)+")", mSubLayer->providerType(), "ogr");
    qDebug( "New vector layer created." );


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
        qDebug( "Features added." );
    }
    else
    {
        qDebug( "Unable to add features." );
    }

   // myLayer->commitChanges();

    // add layer if valid
    if( mNewLayer->isValid() )
    {
        qDebug( "Layer is valid." );
        return true;
    }
    // invalid layer
    else
    {
        qDebug( "Layer is NOT valid." );
    }

    return false;

} // bool Dialog::copyLayer()


void Dialog::transferGeometrytoGeos( QgsVectorLayer *theLayer, unsigned short layer )
{
    // empty geometry
    QgsGeometry *geom = NULL;

    if ( layer == 0 )
    {
        mGeosRef.clear();
    }
    else
    {
        mGeosSub.clear();
    }

    // transfer geometry of each feature from subject layer to geos
    QgsFeature myFeature;
    QgsFeatureRequest request = QgsFeatureRequest(); // default feature request
    QgsVectorLayerFeatureIterator featureIt = QgsVectorLayerFeatureIterator( theLayer, request ); // feature iterator
    featureIt.rewind(); // reset iterator to the start position

    while( featureIt.nextFeature( myFeature ) )
    {
        // geometry of the feature
        geom = myFeature.geometry();

        // transfer qgis geometry to geos (over wkt)
        MyGEOSGeom geos;
        geos.setGEOSGeomFromWKT( geom->exportToWkt().toStdString() );

        //geos.setGEOSgeom( geom->asGeos() );

        // add geometry to the list of geos geometries
        if (layer == 0) // reference layer
        {
            mGeosRef.push_back(geos);
        }
        else
        {
            mGeosSub.push_back(geos);
        }

    }

    qDebug("GEOMETRY TRANSFERED");

} // bool Dialog::transferGeometryToGeos()


bool Dialog::transferGeometryFromGeos()
{

    // AFTER DOING SOMETHING WITH GEOMETRY IN GEOS FORMAT

    // some tests
    int caps = mNewLayer->dataProvider()->capabilities();

    if( !(caps & QgsVectorDataProvider::ChangeGeometries) )
    {
        return false;
    }

    // change geometry in the layer according to GEOS geometry
    QgsGeometry geom;
    QgsFeature myFeature;
    QgsFeatureRequest request = QgsFeatureRequest(); // default feature request
    QgsVectorLayerFeatureIterator featureIt = QgsVectorLayerFeatureIterator( mNewLayer, request ); // feature iterator
    featureIt.rewind(); // reset iterator to the start position

    QgsGeometryMap geomMap;
    TGeomLayer::iterator it = mGeosSub.begin(); // geometry iterator

    // next feature in the layer
    while ( featureIt.nextFeature( myFeature ) )
    {
        qDebug("COUNTING");

        // get new geometry for the feature from geos geometry
        geom.fromWkt( QString::fromStdString( (*it).getWKTGeom()) );
        // insert new geometry to the map of new geometries
        geomMap.insert( myFeature.id(), geom );
        it++;

    }

    if( mNewLayer->dataProvider()->changeGeometryValues( geomMap ) )
    {
        qDebug("GEOMETRY TRANSFERED BACK");
        return true;
    }

    return false;


} // bool Dialog::transferGeometryFromGeos()


void Dialog::vertexSnap()
{

    //initGEOS(); ???
    transferGeometrytoGeos( mRefLayer, 0 );
    transferGeometrytoGeos( mSubLayer, 1 );
    qDebug("GEOMETRY TRANSFERED IN VS");

    // DO SOMETHING WITH GEOMETRY IN GEOS FORMAT

    // ERROR -> symbol lookup error: libqgsconflate.so.1.0.0: undefined symbol: _ZN13VertexSnapperC1Ev
    VertexSnapper vs = VertexSnapper();
    qDebug("VERTEX SNAPPER CREATED");

    // set geometries of layers to vertex snapper
    vs.setRefGeometry( mGeosRef );
    qDebug("SET REF GEOMETRY DONE");
    vs.setSubGeometry( mGeosSub );
    qDebug("SET SUB GEOMETRY DONE");

    // set tolerance distance
    vs.setTolDistance( 100 );
    qDebug("SET TOLERANCE DISTANCE DONE");

    // snap vertices from subject layer to the reference layer
    vs.snap();
    mNewLayer->commitChanges();
    qDebug("SNAP DONE");

    // set new geometry
    mGeosSub = vs.getNewGeometry();
    qDebug("SET NEW GEOMETRY DONE");

    // transfer geometry back
    if ( transferGeometryFromGeos() )
    {
        qDebug("SNAPPING DONE");
    }

    //finishGEOS(); ???

} // void Dialog::vertexSnap()


void Dialog::on_okButton_clicked()
{

    mSubLayer = selectedLayer(1);
    mRefLayer = selectedLayer(0);

    if( copyLayer() )
    {
        qDebug( "Layer was copied." );

        vertexSnap();

        // add new layer to the layer registry
        QgsMapLayerRegistry::instance()->addMapLayers( QList<QgsMapLayer *>() << mNewLayer);

        this->close();
    }
    else
    {
        qDebug( "No layer was copied." );
        QMessageBox::information(0,"Information","No Layer was copied!", QMessageBox::Ok);
    }

} // void Dialog::on_okButton_clicked()


void Dialog::on_closeButton_clicked()
{

    this->close();

} // void Dialog::on_closeButton_clicked()
