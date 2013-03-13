#ifndef MATCHINGGEOMETRY_H
#define MATCHINGGEOMETRY_H

// Geos includes
#include <geos/geom/Geometry.h>

// local includes
#include "geoc.h"

/** Class providing methods for match (indicate corresponding) geometries from two datasets. */

class MatchingGeometry
{

public:

    /** Default constructor
      */
    MatchingGeometry();

    /** Constructor
      */
    MatchingGeometry( TGeomLayer *l1, TGeomLayer *l2, double tol = 1 ): geometrySet1(l1), geometrySet2(l2), tolDistance(tol) {}

    /** Set given dist to tolDistance.
      */
    void setTolDistance( double dist ){ tolDistance = dist; }

    /** Set geometrySet1/2.
      * @param geometrySet Vector of geometries to be set.
      * @param isFirst In case of true geometry is set to geometrySet1 otherwise to geometrySet2.
      */
    void setGeometrySet( TGeomLayer * geometrySet, bool isFirst );

    /** Find geometries from vector 1 or 2 within tolerance distance to given geometry.
      * @param geom Tested geometry.
      * @param isFirst In case of true search for close geometries in geometrySet1 otherwise in geometrySet2.
      * @param set Vector with close geometries.
      */
    void closeGeometries( TGeomLayer * set, const Geometry *geom, bool isFirst );

    /** Test whether two geometries are within tolerance distance.
      @param g1 Geometry of the first tested feature.
      @param g2 Geometry of the  second tested feature.
      @return True if g1 and g2 are within distance tolerance.
     */
    bool isClose( const Geometry * g1, const Geometry * g2 );

    /** Create buffer of given Geometry with tolDistance
      * @param geom Geometry to be buffered.
      * @param buffer Buffer of geometry.
      */
    void buffer( Geometry *buffer, const Geometry *geom );

    /** Create boundary of given Geometry
      * @param geom Geometry which boundary has to be created.
      * @param boundary Boundary of geometry.
      */
    void boundary( Geometry *boundary, const Geometry *geom );

    /** Return true if geomA contains geomB.
      * @param geomA Geometry predicted to be outside.
      * @param geomB Geometry predicted to be inside.
      * @return True if geomB is completely inside geomA.
      */
    bool contains( const Geometry *geomA, const Geometry *geomB );

    /** Sets matching geometry to given geometry.
      * @param geom MyGEOSGeom to which we want to set matching geometry.
      * @param isFirst In case of true search for matching geometry in geometrySet1 otherwise in geometrySet2.
      * @return True if some matching geometry was found.
      */
    bool setMatch( MyGEOSGeom *geom, bool isFirst );

private:

    TGeomLayer* geometrySet1;
    TGeomLayer* geometrySet2;
    double tolDistance;
};

#endif // MATCHINGGEOMETRY_H
