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

        // stop of it is not vector layer
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


// return layer from comboBox
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
    // return layer according to choosed name
    if ( (index == 0) && (vl->name() == mcbReferences->currentText()) )
      return vl;

    if( (index == 1) && (vl->name() == mcbSubjects->currentText()) )
        return vl;
  }

  // return NULL if there is no vector layer
  return NULL;
}

void Dialog::on_okButton_clicked()
{

    // reference layer
    mRefLayer = selectedLayer(0);

    // stop if there is no reference layer
    if( mRefLayer == NULL )
    {
        qDebug("No layer");
        QMessageBox::information(0,"Information","No Layer!", QMessageBox::Ok);
        return;
    }

    // new vector layer as copy of mRefLayer
    QgsVectorLayer *myLayer = new QgsVectorLayer(mRefLayer->source(), mRefLayer->name()+"_copy", mRefLayer->providerType());

    /*// feature from reference layer
    QgsFeature myFeature;

    // copy features from reference layer to the new layer
    while( mRefLayer->nextFeature(myFeature) )
    {
        myLayer->addFeature(myFeature);
    }*/

    // add layer if valid
    if(myLayer->isValid())
    {
        qDebug("Layer is valid");
        // add new layer to the layer registry
        QgsMapLayerRegistry::instance()->addMapLayers( QList<QgsMapLayer *>() << myLayer);
    }
    else
    {
        delete myLayer;
        qDebug("Layer is NOT valid");

    }

}

void Dialog::on_closeButton_clicked()
{
    this->close();
}
