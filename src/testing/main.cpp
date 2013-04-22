#include <cstdlib>
#include <iostream>

#include <QString>

#include <qgsapplication.h>
#include <qgsmaplayer.h>
#include <qgsvectorlayer.h>
#include <qgsproviderregistry.h>

#include <geos_c.h>

#include "qgsconflateprovider.h"

using std::string;
using std::cout;
using std::cerr;
using std::endl;

static int parse_opt(int, const char **, QString &, QString &, QString &);
static QgsVectorLayer *open_layer(QString, QString);

void log_and_exit(const char *fmt, ...) {
    va_list ap;
    fprintf( stdout, "ERROR: ");

    va_start (ap, fmt);
    vfprintf( stdout, fmt, ap);
    va_end(ap);
    fprintf( stdout, "\n" );
    exit(1);
}

void notice(const char *fmt, ...) {
    va_list ap;

    fprintf( stdout, "NOTICE: ");

    va_start (ap, fmt);
    vfprintf( stdout, fmt, ap);
    va_end(ap);
    fprintf( stdout, "\n" );
}

int main(int argc, const char **argv)
{
    initGEOS(notice, log_and_exit);

    QString input_ref, input_sub, output;

    QgsVectorLayer *refLayer;
    QgsVectorLayer *subLayer;
    QgsVectorLayer *newLayer;
    
    QgsConflateProvider *cProvider;
    
    if (0 != parse_opt(argc, argv, input_ref, input_sub, output))
        return EXIT_FAILURE;

    QgsProviderRegistry::instance(QgsApplication::pluginPath()); // some memory error here

    cProvider = new QgsConflateProvider();

    // load ref layer
    refLayer = open_layer(input_ref, "input_ref");
    if (!refLayer)
        exit(EXIT_FAILURE);
    
    // load second input layer
    subLayer = open_layer(input_sub, "input_sub");
    if (!subLayer)
        exit(EXIT_FAILURE);
    
    cProvider->setRefVectorLayer(refLayer);
    cProvider->setSubVectorLayer(subLayer);
    cProvider->setTolDistance(0.01);

    // create output layer
    cProvider->copyLayer(output + ".shp");

    // do something ...
    cProvider->vertexSnap();
    
    finishGEOS();

    delete cProvider;

    exit(EXIT_SUCCESS);
}

int parse_opt(int argc, const char **argv, QString &input_ref, QString &input_sub, QString &output)
{
    if (argc != 4) {
        cerr << "Usage: input_ref input_sub output" << endl;
        return -1;
    }

    input_ref = argv[1];
    input_sub = argv[2];
    output    = argv[3];
    
    return 0;
}

QgsVectorLayer *open_layer(QString uri, QString basename)
{
    QgsVectorLayer *pLayer;
    
    pLayer = new QgsVectorLayer(uri, basename, "ogr");
    if (!pLayer || !pLayer->isValid()) {
        cerr << "Unable to open " << uri.toStdString() << endl;
        return NULL;
    }
    
    return pLayer;
}
