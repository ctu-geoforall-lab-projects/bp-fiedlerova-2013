#ifndef POINTMATCHING_H
#define POINTMATCHING_H

// local includes
#include "mygeosgeom.h"
#include "geoc.h"

// GEOS includes
#include <geos/algorithm/Angle.h>

using namespace geos;
using namespace geos::geom;
using namespace geos::algorithm;
using namespace std;


/** Class providing methods for indicating corresponding points from two layers. */

class PointMatching
{

public:

    /** Default constructor
      */
    PointMatching();

    /** Set given dist to tolDistance.
      */
    void setTolDistance( double dist ){ tolDistance = dist; }

    /** Set pointGeometry1/2.
      * @param pointGeom Vector of point geometries to be set.
      * @param i Index of point geometries to be set to.
      */
    void setPointGeometry( TGeomLayer pointGeom, short i ){ if (i == 1) { pointGeometry1 = pointGeom; } else { pointGeometry2 = pointGeom; } }

    /** Find points from vector of points 1 or 2 within tolerance distance to given point.
      * @param point Coordinate - tested point.
      * @param vid Id of vector of geometry where to find candidates ( 1 for pointGeometry1 and 2 for pointGeometry2).
      * @return Vector with candidate points.
      */
    TGeomLayer findCandidates( Geometry *point, short vid );

    /** Return the sector for given pair of point geometries.
      * @param point1 Coordinate - tested point.
      * @param point2 Coordinate - candidate point.
      * @return Number from 1 to 8 representing the sector of line between points.
      *         1 is for deviation 0°-45° from X, 2 for 45°-90°, 3 for 90°-135° etc.
      */
    int getSector( const Coordinate * point1, const Coordinate * point2 );

    /** Return the sectors for given point and close points.
      * @param point Coordinate - tested point.
      * @param closes Vector of candidates points.
      * @return Vector with sector indicators.
      */
    vector<int> findSectors( const Coordinate * point, TGeomLayer closes);

    /** Compare sectors and return true if they are similar.
      * @param sectors1,2 Vector with indicators of sector.
      * @return true If given sectors are similar.
      */
    bool compareSectors( vector<int> sectors1, vector<int> sectors2 );

    /** Return difference between length of two segments.
      * @param cs1 Coordinate sequence with two points creating first segment.
      * @param cs2 Coordinate sequence with two points creating second segment.
      * @return Length difference between segments.
      */
    double getLengthDiff( CoordinateSequence * cs1, CoordinateSequence * cs2);

    // funkce pro porovnani delek segmentu

    /** Set matching point to the given point from pointGeometry.
      * @param point Geometry of tested point.
      * @param vid Id of vector of geometry where to find matching points ( 1 for pointGeometry1 and 2 for pointGeometry2).
      */
    void setMatchingPoint( MyGEOSGeom * point, short vid );

private:

    TGeomLayer pointGeometry1;
    TGeomLayer pointGeometry2;
    double tolDistance;
};

#endif // POINTMATCHING_H
