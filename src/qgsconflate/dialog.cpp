// Local includes
#include "dialog.h"

// QT includes
#include <QMessageBox>

// QGis includes
#include <qgsmapcanvas.h>
#include <qgsvectorlayer.h>
#include <qgsmaplayerregistry.h>
#include <qgsogrprovider.h>
#include <qgsvectorfilewriter.h>

// GEOS includes
#include <geos_c.h>
#include <geos/export.h>


Dialog::Dialog(QWidget *parent, Qt::WFlags fl, QgisInterface * iface) :
    QDialog(parent, fl)
{
    setupUi(this);

    // set interface
    mIface = iface;

    // initialize data
    mRefLayer = NULL;
    mSubLayer = NULL;

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


bool Dialog::copyLayer()
{
    // reference layer
    mSubLayer = selectedLayer(1);

    // stop if there is no reference layer
    if( mSubLayer == NULL )
    {
        qDebug("No layer");
        QMessageBox::information(0,"Information","No Layer!", QMessageBox::Ok);
        return false;
    }

    // new vector layer as copy of mRefLayer
    QMap<int, int> *oldToNewAttrIdxMap = new QMap<int, int>;

    // uri of new vector layer
    QString uri = "/home/desktop/"+mSubLayer->name()+"_copy.shp";
    QString *error = NULL;
    bool overwrite = false;

    // create empty layer  -----> ERROR
    QgsVectorLayerImport::ImportError ierror = QgsOgrProvider::createEmptyLayer(uri, mSubLayer->pendingFields(), mSubLayer->wkbType(),
                                               &(mSubLayer->crs()), overwrite, oldToNewAttrIdxMap, error);
    if( ierror )
    {
        return false;
    }

    qDebug( "Empty vector layer created." );

    //new layer
    QgsVectorLayer *myLayer = new QgsVectorLayer(mSubLayer->source(), mSubLayer->name()+"_copy", mSubLayer->providerType());
    qDebug( "New vector layer created." );

    // copy features from subject layer to the new layer
    QgsFeature myFeature;
    while( mSubLayer->nextFeature(myFeature) )
    {
        // add feature
        if ( myLayer->addFeature(myFeature, true) )
        {
            continue;
        }
        else
        {
            qDebug( "Unable to add feature." );
        }

    }

    // add layer if valid
    if(myLayer->isValid())
    {
        qDebug("Layer is valid");
        // add new layer to the layer registry
        QgsMapLayerRegistry::instance()->addMapLayers( QList<QgsMapLayer *>() << myLayer);
        return true;
    }
    // invalid layer
    else
    {
        delete myLayer;
        qDebug("Layer is NOT valid");
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

    while( theLayer->nextFeature(myFeature) )
    {
        // geometry of the feature
        geom = myFeature.geometry();

        // transfer qgis geometry to geos
        MyGEOSGeom geos;
        geos.setGEOSgeom( geom->asGeos() );

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


} // bool Dialog::transferGeometryToGeos()


void Dialog::transferGeometryFromGeos( )
{

    // AFTER DOING SOMETHING WITH GEOMETRY IN GEOS FORMAT

    QgsGeometry *geom = NULL;

    // change geometry in the layer according to GEOS geometry
    QgsFeature myFeature;

    for( TGeomLayer::iterator it = mGeosSub.begin(); mGeosSub.end() != it; it++ )
    {
        // next feature in the layer
        if ( mSubLayer->nextFeature(myFeature) )
        {
            // set new geometry to the feature from geos geometry
            geom->fromGeos( (*it).getGEOSGeom() );
            mSubLayer->changeGeometry( myFeature.id(), geom );
        }

    }

} // bool Dialog::transferGeometryFromGeos()


void Dialog::vertexSnap()
{

    //initGEOS(); ???
    transferGeometrytoGeos( mSubLayer, 1 );
    transferGeometrytoGeos( mRefLayer, 0 );

    // DO SOMETHING WITH GEOMETRY IN GEOS FORMAT
    VertexSnapper vs;

    // set geometries of layers to vertex snapper
    vs.setRefGeometry( mGeosRef );
    vs.setSubGeometry( mGeosSub );

    // set tolerance distance
    vs.setTolDistance( 1 );

    // snap vertices from subject layer to the reference layer
    vs.snap();

    //finishGEOS(); ???

} // void Dialog::vertexSnap()


void Dialog::on_okButton_clicked()
{

    if( copyLayer() )
    {
        vertexSnap();
        qDebug("Layer was copied.");
    }
    else
    {
        qDebug("No layer was copied.");
        QMessageBox::information(0,"Information","No Layer was copied!", QMessageBox::Ok);
    }

} // void Dialog::on_okButton_clicked()


void Dialog::on_closeButton_clicked()
{
    this->close();
} // void Dialog::on_closeButton_clicked()
