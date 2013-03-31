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

    // conflate provider
    mConflate = new QgsConflateProvider();

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
    delete mConflate;
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


void Dialog::on_processButton_clicked()
{
    // clear protocol
    this->mTextEdit->clear();
    this->mlabelStatus->setText("Conflating, please, wait.");

    // set values to Conflate provider
    mConflate->setTolDistance( this->mSpinBoxDist->value() );
    mConflate->setRefVectorLayer( selectedLayer(0) );
    mConflate->setSubVectorLayer( selectedLayer(1) );

    this->mlabelStatus->setText("Copying subject layer.");
    // copy subject layer and conflate
    if( mConflate->copyLayer() )
    {
        qDebug( "Dialog::onokButtonclicked: Layer was copied." );

        if ( mrbSnapVertex->isChecked() )
        {
            mConflate->vertexSnap();
        }
        else if ( mrbSnapFeature->isChecked() )
        {
            mConflate->featureSnap();

        }
        else if ( mrbConflate->isChecked() )
        {
            mConflate->align();
        }

        QgsVectorLayer *myLayer = mConflate->getNewVectorLayer();

        if ( myLayer && myLayer->isValid())
        {
            // add new layer to the layer registry
            QgsMapLayerRegistry::instance()->addMapLayers( QList<QgsMapLayer *>() << myLayer);
        }
        else
        {
            QMessageBox::information(0,"Information","New layer was created but cannot be added to the layer tree"
                                     "because it is not valid. Please add it manually.", QMessageBox::Ok);
        }

        // set protocol
        mProtocol = mConflate->getProtocol();
        this->mTextEdit->setPlainText(mProtocol);
    }
    else
    {
        qDebug( "Dialog::on_okButton_clicked: Unable to copy subject layer for conflation." );
        QMessageBox::information(0,"Information","Unable to copy subject layer for conflation.", QMessageBox::Ok);
    }

    this->mlabelStatus->setText("Conflation done.");

} // void Dialog::on_okButton_clicked()


void Dialog::on_closeButton_clicked()
{

    this->close();

} // void Dialog::on_closeButton_clicked()
