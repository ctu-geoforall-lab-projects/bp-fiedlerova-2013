#ifndef QGSCONFLATEPLUGIN_H
#define QGSCONFLATEPLUGIN_H

#ifndef QgsConflatePlugin_H
#define QgsConflatePlugin_H

//QGIS includes
#include <QObject>
#include <QAction>

//Qt includes
#include <qgisplugin.h>

//Local includes
#include "dialog.h"

/** Class for creating plugin and loading it to QGIS. */

class QgsConflatePlugin : public QObject, public QgisPlugin
{

    Q_OBJECT

    public:

        QgsConflatePlugin(QgisInterface *iface);
        ~QgsConflatePlugin();

        void initGui();
        void unload();

    private slots:

        void showDialog();

    private:

        QgisInterface *mIface;
        QAction *mAction;
        Dialog *mDialog;

};

#endif // QgsConflatePlugin_H

#endif // QGSCONFLATEPLUGIN_H
