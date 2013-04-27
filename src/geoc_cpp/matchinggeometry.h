/***************************************************************************
    matchinggeometry.h

    GEOC - GEOS Conflation library

    ---------------------
    begin                : April 2013
    copyright            : (C) 2013 by Tereza Fiedlerová
    email                : tfiedlerova dot at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This is free software; you can redistribute it and/or modify it       *
 *   under the terms of the GNU General Public License as published by     *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

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

using namespace geoc;
using namespace geoc::geo;
using namespace geoc::alg;
using namespace geoc::idx;

namespace geoc {
namespace alg {

/** Class providing methods for match (indicate corresponding) geometries from two datasets. */

class MatchingGeometry
{

public:

    /** Default constructor
      */
    MatchingGeometry();

    /** Constructor
      */
    MatchingGeometry( TGeomLayer *l, double tol = 0 ): geometrySet(l), tolDistance(tol), sIndex(NULL) {}

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

    /** Sets matching geometry from geometrySet to given geometry.
      * @param geom GEOCGeom to which we want to set matching geometry.
      * @return True if some matching geometry was found.
      */
    bool setMatch( GEOCGeom *geom );

private:

    TGeomLayer* geometrySet;
    vector<Geometry*> closeSet;
    double tolDistance;
    SpatialIndex *sIndex;

    /** Find geometries from vector within tolerance distance to given geometry.
      * @param geom Tested geometry.
      * @param geomSet Vector with close geometries.
      */
    void closeGeometries( const Geometry *geom );

    /** Test whether two geometries are within tolerance distance.
      * @param g1 Geometry of the first tested feature.
      * @param g2 Geometry of the  second tested feature.
      * @return True if g1 and g2 are within distance tolerance.
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

};

} // namespace alg
} // namespace geoc

#endif // MATCHINGGEOMETRY_H
