#ifndef DIALOG_H
#define DIALOG_H

// Qt includes
#include <QDialog>
#include <QtGui>

// QGis includes
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
    QgsVectorLayer* selectedLayer( int index );

    /** Create empty layer to the given uri.
      * @param uri Uri of new empty layer.
      * @true if creation was succesfull
      */
    bool createEmptyLayer( QString uri );

    /** Creates new uri to according to given parameters.
      * @param path Path to the directory where new file should be created ended by base name of new file.
      * @param rank Number used for file name.
      * @return new uri to create new file
      */
    QString newUri( QString path, int &rank);

    /** Check if file (source) exists.
      * @param source Path to the file.
      * @return true if there is file at this source
      */
    bool fileExists( QString source );

    /** Copy subject layer to the mNewLayer.
     *  @return true if copied layer is valid
     */
    bool copyLayer();

    /** Transfers qgis geometry to geos.
     *  @param theLayer Layer which geometry has to be transfered.
     */
    void transferGeometrytoGeos( QgsVectorLayer *theLayer, unsigned short layer );

    /** Transfers geos geometry to qgis.
      * @return true if transfering was succesfull
     */
    bool transferGeometryFromGeos( );

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
    QgsVectorLayer *mNewLayer;
    TGeomLayer mGeosSub;
    TGeomLayer mGeosRef;

};

#endif // DIALOG_H
