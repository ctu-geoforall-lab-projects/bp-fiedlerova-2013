#ifndef VERTEXSNAPPER_H
#define VERTEXSNAPPER_H

// GEOS includes
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/util/GeometryEditor.h>

// std includes
# include <vector>

// local includes
#include "mygeosgeom.h"


using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace geos::geom::util;


typedef std::vector< MyGEOSGeom > TGeomLayer;


/** Class to snap vertices of one layer to the close vertices of the other layer. */

class VertexSnapper
{

public:
    VertexSnapper();

    /** Set reference layer geometries.
      @param ref Vector of geometries in reference layer.
     */
    void setRefGeometry(TGeomLayer ref){ refGeometry = ref; }

    /** Set subject layer geometries.
      @param ref Vector of geometries in subject layer.
     */
    void setSubGeometry(TGeomLayer sub){ subGeometry = sub; }

    /** Set distance tolerance for snapping.
      @param tol Maximal distance for snapping.
     */
    void setTolDistance(double tol){ tolDistance = tol; }

    /** Get new geometry of subject layer.
      @return Changed geometry of subject layer.
     */
    TGeomLayer getNewGeometry(){ return newGeometry; }

    /** Snap close vertices of subject layer to these of reference layer.
     */
    void snap();

    /** Test whether two geometries are within tolerance distance.
      @param g1 Geometry of the first tested feature.
      @param g2 Geometry of the  second tested feature.
      @return true if g1 and g2 are within distance tolerance.
     */
    bool isClose(MyGEOSGeom g1, MyGEOSGeom g2);

    /** Snap vertices of given geometry to the close vertices from given coordinate sequence.
      @param geom Pointer to the tested geometry.
      @param closeCoord Coordinate sequence with close point from the reference layer.
     */
    void snapVertices( MyGEOSGeom * geom, CoordinateSequence & closeCoord );

    /** Edit geometry according to the given coordinates.
      @param geom Pointer to the tested geometry.
      @param coord Coordinate sequence with new points.
     */
    void editGeometry( MyGEOSGeom * geom, CoordinateSequence & coord );

private:

    TGeomLayer refGeometry;
    TGeomLayer subGeometry;
    TGeomLayer newGeometry;
    double tolDistance;

};

#endif // VERTEXSNAPPER_H
