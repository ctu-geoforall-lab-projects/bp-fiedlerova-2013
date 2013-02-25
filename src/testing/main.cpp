#include <cstdlib>
#include <iostream>

#include <QString>

#include <qgsmaplayer.h>
#include <qgsvectorlayer.h>
#include <qgsproviderregistry.h>

using std::string;
using std::cout;
using std::cerr;
using std::endl;

static int parse_opt(int, const char **, QString &, QString &, QString &);
static QgsVectorLayer *open_layer(QString, QString);

int main(int argc, const char **argv)
{
    QString input_ref, input_sub, output;

    QgsVectorLayer *refLayer;
    QgsVectorLayer *subLayer;
    QgsVectorLayer *newLayer;
    
    // todo
    // ConflateProvider cProvider;
    
    if (0 != parse_opt(argc, argv, input_ref, input_sub, output))
        return EXIT_FAILURE;

    QgsProviderRegistry::instance("/opt/Quantum-GIS/build/output/lib/qgis/plugins/");

    // todo
    // cProvider = new ConflateProvider();

    // load ref layer
    refLayer = open_layer(input_ref, "input_ref");
    if (!refLayer)
        exit(EXIT_FAILURE);
    
    // load second input layer
    subLayer = open_layer(input_sub, "input_sub");
    if (!subLayer)
        exit(EXIT_FAILURE);
    
    // create output layer
    // cProvider->createEmptyLayer();

    // do something ...

    // save 
    
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
