/***************************************************************************
    qgsconflateplugin.cpp

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

//Qt includes
#include <QTranslator>

//QGIS includes
#include <qgisinterface.h>

//Local includes
#include "qgsconflateplugin.h"


#ifdef WIN32
    #define QGISEXTERN extern "C" __declspec( dllexport )
#else
    #define QGISEXTERN extern "C"
#endif


// static members
static const char * const sName = "Conflate Plugin";
static const char * const sDescription = "dialog for conflation";
static const char * const sVersion = "0.1";
static const QString sCategory = QObject::tr("Vector");
static const QgisPlugin::PLUGINTYPE sType= QgisPlugin::UI;


QgsConflatePlugin::QgsConflatePlugin(QgisInterface *iface)
    : QgisPlugin(sName, sDescription, sCategory, sVersion, sType),
      mIface(iface)
{


} // constructor


QgsConflatePlugin::~QgsConflatePlugin()
{

} // destructor


void QgsConflatePlugin::initGui()
{
   mAction = new QAction( QIcon(":/icon/icons/mActionConflate.svg"), tr("&Conflate"), this);
   connect(mAction, SIGNAL(activated()), this, SLOT(showDialog()));
   mIface->addToolBarIcon(mAction); // icon in toolbar "Plugins"
   mIface->addPluginToMenu(tr("&Conflate"), mAction);  // Plugin menu
} // add plugin elements to toolbar and menu


void QgsConflatePlugin::unload()
{
    mIface->removeToolBarIcon(mAction);
    mIface->removePluginMenu(tr("&Conflate"), mAction);
    delete mAction;
    delete mDialog;
} // remove allocated GUI elements


void QgsConflatePlugin::showDialog()
{
    mDialog = new Dialog(0,0, mIface);
    mDialog->setModal(false);
    mDialog->show();
    return;

} // show dialog window with Conflate menu


// PLUGIN GENERATOR
QGISEXTERN QgisPlugin* classFactory(QgisInterface* iface)
{
    return new QgsConflatePlugin(iface);
}

QGISEXTERN QString name()
{
    return sName;   // name in Plugin Manager
}

QGISEXTERN QString description()
{
    return sDescription;  // description in Plugin Manager
}

QGISEXTERN QString version()
{
    return sVersion;
}

QGISEXTERN QString category()
{
    return sCategory;
}


// Plugin type (UI or MapLayer plugin)
QGISEXTERN int type()
{
    return sType;
}

// Remove plugin
QGISEXTERN void unload(QgisPlugin* theQgsConflatePluginPointer)
{
    delete theQgsConflatePluginPointer;
}

