#ifndef MATCHINGGEOMETRY_H
#define MATCHINGGEOMETRY_H

// Geos includes
#include <geos/geom/Geometry.h>
#include <geos/geom/Envelope.h>
#include <geos/index/strtree/STRtree.h>

// local includes
#include "geoc.h"

using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace geos::index;
using namespace geos::index::strtree;


/** Class providing methods for match (indicate corresponding) geometries from two datasets. */

class MatchingGeometry
{

public:

    /** Default constructor
      */
    MatchingGeometry();

    /** Constructor
      */
    MatchingGeometry( TGeomLayer *l, double tol = 1 ): geometrySet(l), tolDistance(tol) {}

    /** Destructor
      */
    ~MatchingGeometry();

    /** Set given dist to tolDistance.
      */
    void setTolDistance( double dist ){ tolDistance = dist; }

    /** Set geometrySet where to find corresponding features.
      * @param geometrySet Vector of geometries to be set.
      */
    void setGeometrySet( TGeomLayer * geomSet );

    /** Find geometries from vector within tolerance distance to given geometry.
      * @param geom Tested geometry.
      * @param geomSet Vector with close geometries.
      */
    void closeGeometries( const Geometry *geom );

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
    Geometry * buffer( Geometry *geom );

    /** Create boundary of given Geometry
      * @param geom Geometry which boundary has to be created.
      * @param boundary Boundary of geometry.
      */
    Geometry * boundary( Geometry *geom );

    /** Return true if geomA contains geomB.
      * @param geomA Geometry predicted to be outside.
      * @param geomB Geometry predicted to be inside.
      * @return True if geomB is completely inside geomA.
      */
    bool contains( const Geometry *geomA, const Geometry *geomB );

    /** Sets matching geometry from geometrySet to given geometry.
      * @param geom MyGEOSGeom to which we want to set matching geometry.
      * @return True if some matching geometry was found.
      */
    bool setMatch( MyGEOSGeom *geom );

    /** Build spatial index
      */
    void buildIndex();

private:

    TGeomLayer* geometrySet;
    vector<Geometry*> closeSet;
    double tolDistance;
    SpatialIndex *index;

};

#endif // MATCHINGGEOMETRY_H
