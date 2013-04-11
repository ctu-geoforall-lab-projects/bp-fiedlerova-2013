#ifndef VERTEXSNAPPER_H
#define VERTEXSNAPPER_H

// GEOS includes
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/util/GeometryEditor.h>
#include <geos/geom/LineString.h>
#include <geos/geom/Point.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/Polygon.h>
#include <geos/operation/distance/DistanceOp.h>
#include <geos/geom/Envelope.h>
#include <geos/index/strtree/STRtree.h>

// std includes
# include <vector>

// local includes
#include "geoc.h"

using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace geos::geom::util;
using namespace geos::operation::distance;
using namespace geos::index;
using namespace geos::index::strtree;

using namespace geoc;
using namespace geoc::geo;
using namespace geoc::alg;
using namespace geoc::edit;


namespace geoc {
namespace alg {

typedef std::vector< GEOCGeom > TGeomLayer;

/** Class to snap vertices of one layer to the close vertices of the other layer. */

class VertexSnapper
{

public:

    /** Default constructor.
      */
    VertexSnapper();

    /** Destructor.
      */
    ~VertexSnapper();

    /** Set reference layer geometries.
      * @param ref Vector of geometries in reference layer.
      */
    void setRefGeometry(TGeomLayer & ref){ refGeometry = ref; }

    /** Set subject layer geometries.
      * @param sub Vector of geometries in subject layer.
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

    /** Snap close vertices of subject layer to these of reference layer.
     */
    void snap();

    /** Snap vertices of given geometry to the close vertices from given coordinate sequence.
      * @param geom Pointer to the tested geometry.
      * @param closeCoord Coordinate sequence with close point from the reference layer.
      */
    void snapVertices( GEOCGeom * geom, CoordinateSequence * closeCoord );

    /** Build spatial index.
      */
    void buildIndex();

    /** Get list of invalid geometries.
      */
    vector<int> getInvalidGeometries() { return invalids; }

private:

    TGeomLayer refGeometry;
    TGeomLayer subGeometry;
    TGeomLayer newGeometry;
    double tolDistance;
    SpatialIndex* sIndex;
    vector<int> invalids;

};

} //namespace geoc
} //namespace alg

#endif // VERTEXSNAPPER_H
