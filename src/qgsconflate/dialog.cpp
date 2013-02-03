// Local includes
#include "dialog.h"

// QT includes
#include <QMessageBox>

// QGis includes
#include <qgis/qgsmapcanvas.h>
#include <qgis/qgsvectorlayer.h>
#include <qgis/qgssinglesymbolrendererv2.h>
#include <qgis/qgssymbolv2.h>
#include <qgis/qgsmaplayerregistry.h>
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
    mRefLayer = selectedLayer(0);

    // stop if there is no reference layer
    if( mRefLayer == NULL )
    {
        qDebug("No layer");
        QMessageBox::information(0,"Information","No Layer!", QMessageBox::Ok);
        return false;
    }

    // new vector layer as copy of mRefLayer
    QgsVectorLayer *myLayer = new QgsVectorLayer(mRefLayer->source(), mRefLayer->name()+"_copy", mRefLayer->providerType());

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


void Dialog::transferGeometrytoGeos( QgsVectorLayer *theLayer )
{
    // empty geometry
    QgsGeometry *geom = NULL;
    mGeosList.clear();

    // transfer geometry of each feature from subject layer to geos
    QgsFeature myFeature;

    while( theLayer->nextFeature(myFeature) )
    {
        // geometry of the feature
        geom = myFeature.geometry();

        // transfer qgis geometry to geos
        GEOSGeometry * geos = geom->asGeos();

        // add geometry to the list of geos geometries
        mGeosList.push_back(geos);

    }


} // bool Dialog::transferGeometryToGeos()


void Dialog::transferGeometryFromGeos( )
{

    // AFTER DOING SOMETHING WITH GEOMETRY IN GEOS FORMAT

    QgsGeometry *geom = NULL;

    // change geometry in the layer according to GEOS geometry
    QgsFeature myFeature;

    for( std::vector<GEOSGeometry *>::iterator it = mGeosList.begin(); mGeosList.end() != it; it++ )
    {
        // next feature in the layer
        if ( mSubLayer->nextFeature(myFeature) )
        {
            // set new geometry to the feature from geos geometry
            geom->fromGeos(*it);
            mSubLayer->changeGeometry(myFeature.id(), geom);
        }

    }

} // bool Dialog::transferGeometryFromGeos()


void Dialog::vertexSnap()
{

    //initGEOS(); ???

    // DO SOMETHING WITH GEOMETRY IN GEOS FORMAT

    //finishGEOS(); ???

} // void Dialog::vertexSnap()


void Dialog::on_okButton_clicked()
{

    if( copyLayer() )
    {
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
