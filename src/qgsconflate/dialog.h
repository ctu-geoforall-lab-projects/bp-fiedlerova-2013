#ifndef DIALOG_H
#define DIALOG_H

// Qt includes
#include <QDialog>
#include <QColorDialog>

// QGis includes
#include <QtGui>
#include <qgis/qgisinterface.h>
#include <qgis/qgsmaplayer.h>

// Local includes
#include "ui_dialog.h"

class Dialog : public QDialog, private Ui::Dialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = 0, Qt::WFlags fl = 0, QgisInterface *iface = 0);
    ~Dialog();

    // return selected layer
    QgsVectorLayer* selectedLayer(int index);

private slots:
    void on_okButton_clicked();

    void on_closeButton_clicked();

private:
    QgsMapLayer *mMapLayer;
    QgisInterface *mIface;
    QgsVectorLayer *mRefLayer;
    QgsVectorLayer *mSubLayer;

};

#endif // DIALOG_H
