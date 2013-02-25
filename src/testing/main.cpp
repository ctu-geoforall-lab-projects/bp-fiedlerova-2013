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

static int parse_opt(int, const char **, QString &, QString &);

int main(int argc, const char **argv)
{
    QString input_ref, input_sub, output;

    QgsVectorLayer *refLayer;
    QgsVectorLayer *subLayer;
    QgsVectorLayer *newLayer;
    
    // todo
    // ConflateProvider cProvider;
    
    if (0 != parse_opt(argc, argv, input_ref, input_sub))
        return EXIT_FAILURE;

    output = input_ref + "_" + input_sub;
    cout << "Input: " << input_ref.toStdString() << " " << input_sub.toStdString() <<
        " -> " << output.toStdString()<< endl;

    QgsProviderRegistry::instance("/opt/Quantum-GIS/build/output/lib/qgis/plugins/");

    // todo
    // cProvider = new ConflateProvider();

    // load ref layer
    refLayer = new QgsVectorLayer(input_ref, "input_ref", "ogr");
    if (!refLayer || !refLayer->isValid()) {
        cerr << "Unable to open " << input_ref.toStdString() << endl;
        return EXIT_FAILURE;
    }

    // load second input layer
    subLayer = new QgsVectorLayer(input_sub, "input_sub", "ogr");
    if (!subLayer || !subLayer->isValid()) {
        cerr << "Unable to open " << input_sub.toStdString() << endl;
        return EXIT_FAILURE;
    }
        
    // create output layer
    // cProvider->createEmptyLayer();

    // do something ...

    // save 
    
    return EXIT_SUCCESS;
}

int parse_opt(int argc, const char **argv, QString &input_ref, QString &input)
{
    if (argc != 3) {
        cerr << "Invalid input: provide two vector maps" << endl;
        return -1;
    }

    input_ref = argv[1];
    input     = argv[2];
    
    return 0;
}
