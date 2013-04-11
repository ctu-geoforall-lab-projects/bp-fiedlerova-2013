#ifndef FEATURESNAPPER_H
#define FEATURESNAPPER_H

// std includes
# include <vector>

// geos includes
#include <geos/geom/Envelope.h>
#include <geos/index/strtree/STRtree.h>

// local includes
#include "geoc.h"

using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace geos::index;
using namespace geos::index::strtree;

using namespace geoc;
using namespace geoc::geo;
using namespace geoc::alg;

namespace geoc {
namespace alg {

/** Class to snap features of one layer to the close features of the other layer. */

class FeatureSnapper
{

public:

    /** Default constructor.
      */
    FeatureSnapper();

    /**
      */
    ~FeatureSnapper();

    /** Set reference layer geometries.
      * @param ref Vector of geometries in reference layer.
      */
    void setRefGeometry(TGeomLayer & ref){ refGeometry = ref; }

    /** Set subject layer geometries.
      * @param ref Vector of geometries in subject layer.
      */
    void setSubGeometry(TGeomLayer & sub){ subGeometry = sub; }

    /** Set distance tolerance for snapping.
      * @param tol Maximal distance for snapping.
      */
    void setTolDistance(double tol){ tolDistance = tol; }

    /** Get new geometry of subject layer.
      * @return Changed geometry of subject layer.
      */
    TGeomLayer & getNewGeometry(){ return newGeometry; }

    /** Snap close features of subject layer to these of reference layer.
     */
    void snap();

    /** Test whether two geometries are within tolerance distance.
      * @param g1 Geometry of the first tested feature.
      * @param g2 Geometry of the  second tested feature.
      * @return true if g1 and g2 are within distance tolerance.
      */
    bool isClose(GEOCGeom & g1, GEOCGeom & g2);

    /** Snap vertices of given geometry to the close vertices from given coordinate sequence.
      * @param geom Pointer to the tested geometry.
     */
    void snapFeatures( GEOCGeom * geom,  TGeomLayer *closeFeatures );

    /** Edit geometry according to the given coordinates.
      * @param geom Pointer to the tested geometry.
     */
    void editGeometry( GEOCGeom * geom );

    /** Build spatial index.
      */
    void buildIndex();

private:

    TGeomLayer refGeometry;
    TGeomLayer subGeometry;
    TGeomLayer newGeometry;
    double tolDistance;
    SpatialIndex *index;
    vector<int> invalids;

};

} //namespace geoc
} //namespace alg

#endif // FEATURESNAPPER_H
