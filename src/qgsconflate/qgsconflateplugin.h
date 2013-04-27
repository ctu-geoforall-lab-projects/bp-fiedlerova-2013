/***************************************************************************
    qgsconflateplugin.h

    QgsConflatePlugin
    plugin for vector-to-vector conflation

    ---------------------
    begin                : April 2013
    copyright            : (C) 2013 by Tereza Fiedlerová
    email                : tfiedlerova dot at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

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
