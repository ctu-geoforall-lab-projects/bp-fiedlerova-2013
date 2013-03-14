#ifndef QGSCONFLATEPROVIDER_H
#define QGSCONFLATEPROVIDER_H

// QGis includes
#include <qgsmaplayer.h>
#include <qgsvectorlayer.h>

// Local includes
#include "geoc.h"

class QgsConflateProvider
{
public:
    QgsConflateProvider();
    //~QgsConflateProvider();

    /** Sets reference layer mRefLayer.
      */
    void setRefVectorLayer( QgsVectorLayer *ref ){ mRefLayer = ref; }

    /** Sets subject layer mSubLayer.
      */
    void setSubVectorLayer( QgsVectorLayer *sub ){ mSubLayer = sub; }

    /** Return new layer - modified subject layer.
      */
    QgsVectorLayer* getNewVectorLayer(){ return mNewLayer; }

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
    bool copyLayer( QString uri = "");

    /** Transfers qgis geometry to geos.
     *  @param theLayer Layer which geometry has to be transfered.
     */
    void transferGeometrytoGeos( bool isRefLayer );

    /** Transfers geos geometry to qgis.
      * @return true if transfering was succesfull
     */
    bool transferGeometryFromGeos( );

    /** Snap vertices */
    void vertexSnap();

    /** Snap features */
    void featureSnap();

    /** Set distance tolerance for snapping.
      */
    void setTolDistance( double dist ){ tolDistance = dist; }

private:

    QgsVectorLayer *mRefLayer;
    QgsVectorLayer *mSubLayer;
    QgsVectorLayer *mNewLayer;
    TGeomLayer mGeosSub;
    TGeomLayer mGeosRef;
    TGeomLayer mGeosNew;
    double tolDistance;
};

#endif // QGSCONFLATEPROVIDER_H
