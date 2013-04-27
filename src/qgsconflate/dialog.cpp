// Local includes
#include "dialog.h"

// Qt includes
#include <QMessageBox>

// QGIS includes
#include <qgsmaplayerregistry.h>
#include <qgsogrprovider.h>
#include <qgsvectorfilewriter.h>
#include <qgsvectorlayerfeatureiterator.h>

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

    // load layers to combo boxes
    loadLayers();

    this->msaHelp->hide();
    help = false;

} // constructor


Dialog::~Dialog()
{

    delete mConflate;

}  // destructor


void Dialog::loadLayers()
{

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

} // void Dialog::loadLayers()


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


bool Dialog::setConflation()
{
    QgsVectorLayer* refLayer = selectedLayer(0);
    QgsVectorLayer* subLayer = selectedLayer(1);

    if ( !refLayer || !subLayer )
    {
        return false;
    }

    // set values to Conflate provider
    mConflate->setTolDistance( this->mSpinBoxDist->value() );
    mConflate->setRefVectorLayer( refLayer );
    mConflate->setSubVectorLayer( subLayer );

    if ( !mConflate->checkLayersType() )
    {
        QMessageBox::warning( 0, tr("Warnig"), tr("Geometry type warning. (Layers must "
                             "have the same geometry type. Line is needed for Match Lines."),
                              QMessageBox::Ok);
        return false;
    }

    if ( this->mLineEdit->text().isEmpty() )
    {
        QMessageBox::information( 0, tr("Information"), tr("No output selected, new layer "
                                  "will be saved to current directory."), QMessageBox::Ok);
    }

    return mConflate->copyLayer( this->mLineEdit->text() );

} // bool Dialog::setConflation()


void Dialog::conflate()
{
    // do conflation according to selected method
    if ( mrbSnapVertex->isChecked() )
    {
        mConflate->setRepair( this->mchbRepair->isChecked() );
        mConflate->vertexSnap();
    }
    else if ( mrbCovAlign->isChecked() )
    {
        mConflate->setRepair( this->mchbRepair->isChecked() );
        mConflate->align();
    }
    else if ( mrbLineMatch->isChecked() )
    {
        mConflate->setMatchCriterium( this->mSpinBoxMatch->value()/100.0);
        mConflate->lineMatch();
    }


} // void Dialog::conflate()


void Dialog::addLayer()
{
    QgsVectorLayer *myLayer = mConflate->getNewVectorLayer();

    if ( myLayer && myLayer->isValid())
    {
        // add new layer to the layer registry
        QgsMapLayerRegistry::instance()->addMapLayers( QList<QgsMapLayer *>() << myLayer);
        QMessageBox::information( 0, tr("Information"), tr("Conflation done. See protocol "
                                  "for further information"), QMessageBox::Ok);
    }
    else
    {
        QMessageBox::warning( 0, tr("Warning"), tr("New layer was created but cannot "
                              "be added to the layer tree because it is not valid. Please"
                              " add it manually to repair."), QMessageBox::Ok);
    }

} // void Dialog::addLayer()


void Dialog::on_processButton_clicked()
{
    this->update();

    // clear protocol
    this->mTextEdit->clear();

    if ( mcbReferences->count() == 0 || mcbSubjects->count() == 0 )
    {
       QMessageBox::information( 0, tr("Information"), tr("Please, add layers for conflation "
                              "to current project first."), QMessageBox::Ok);
    }
    else if ( !mrbSnapVertex->isChecked() && !mrbCovAlign->isChecked() && !mrbLineMatch->isChecked() )
    {
        QMessageBox::information( 0, tr("Information"), tr("Please select conflation "
                              "method."), QMessageBox::Ok);
    }
    else
    {
        // copy subject layer and conflate
        if( setConflation() )
        {
            // change cursor
            this->setCursor(Qt::WaitCursor);

            // do conflation and show result
            conflate();
            addLayer();

            // restore cursor
            this->setCursor(Qt::ArrowCursor);

            // set protocol
            mProtocol = mConflate->getProtocol();
            this->mTextEdit->setPlainText(mProtocol);
        }
        else
        {
            QMessageBox::warning( 0, tr("Warning"), tr("Unable to copy subject layer for "
                                      "conflation. Conflation failed."), QMessageBox::Ok);
        }
    }

} // void Dialog::on_okButton_clicked()


void Dialog::on_closeButton_clicked()
{

    this->close();

} // void Dialog::on_closeButton_clicked()


void Dialog::on_selectButton_clicked()
{

    // select file where to save result
    QString filename = QFileDialog::getSaveFileName( this, tr("Save shapefile"), "",
                                                     tr("(*.shp);;All Files (*)"));

    // set name to the text field
    this->mLineEdit->setText(filename);

} // void Dialog::on_selectButton_clicked()


void Dialog::on_refreshButton_clicked()
{
    // clear
    mLineEdit->clear();
    mTextEdit->clear();

    mcbReferences->clear();
    mcbSubjects->clear();

    // refresh list of layers
    loadLayers();

} // void Dialog::on_refreshButton_clicked()


void Dialog::on_mrbLineMatch_toggled(bool checked)
{
    this->mLabelMatch->setEnabled(checked);
    this->mSpinBoxMatch->setEnabled(checked);
    this->mchbRepair->setEnabled(!checked);

} // void Dialog::on_mrbLineMatch_toggled(bool checked)


void Dialog::on_helpButton_clicked()
{
    if (!help)
    {
        this->msaHelp->show();
        this->helpButton->setText("Hide help <<");
        help = true;
    }
    else
    {
        this->msaHelp->hide();
        this->helpButton->setText("Show help >>");
        help = false;
    }

} // void Dialog::on_helpButton_clicked()
