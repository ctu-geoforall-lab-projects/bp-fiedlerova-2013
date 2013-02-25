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
#include "qgsconflateprovider.h"
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


private slots:

    /** Copy reference layer. */
    void on_okButton_clicked();

    /** Close dialog. */
    void on_closeButton_clicked();

private:

    QgisInterface *mIface;
    QgsConflateProvider* mConflate;


};

#endif // DIALOG_H
