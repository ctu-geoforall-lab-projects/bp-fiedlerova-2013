#ifndef DIALOG_H
#define DIALOG_H

// Qt includes
#include <QDialog>
#include <QColorDialog>

// QGis includes
#include <QtGui>
#include <qgisinterface.h>
#include <qgsmaplayer.h>
#include <qgsgeometry.h>

// Local includes
#include "ui_dialog.h"
#include "geoc.h"

class Dialog : public QDialog, private Ui::Dialog
{
    Q_OBJECT

public:

    Dialog(QWidget *parent = 0, Qt::WFlags fl = 0, QgisInterface *iface = 0);
    ~Dialog();

    /** Returns selected layer.
     *  @param index Index of the layer - 0 for reference, 1 for subject
     *  @return selected layer as QgsVectorLayer *
     */
    QgsVectorLayer* selectedLayer(int index);

    /** Duplicate reference layer.
     *  @return true if copied layer is valid
     */
    bool copyLayer();

    /** Transfers qgis geometry to geos.
     *  @param theLayer Layer which geometry has to be transfered.
     */
    void transferGeometrytoGeos( QgsVectorLayer *theLayer, unsigned short layer );

    /** Transfers geos geometry to qgis.
     */
    void transferGeometryFromGeos( );

    /** Do something with geos geometry */
    void vertexSnap();

private slots:

    /** Copy reference layer. */
    void on_okButton_clicked();

    /** Close dialog. */
    void on_closeButton_clicked();

private:

    QgisInterface *mIface;
    QgsVectorLayer *mRefLayer;
    QgsVectorLayer *mSubLayer;
    TGeomLayer mGeosSub;
    TGeomLayer mGeosRef;

};

#endif // DIALOG_H
