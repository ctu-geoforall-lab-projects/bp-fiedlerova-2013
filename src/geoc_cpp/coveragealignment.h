/***************************************************************************
    coveragealignment.h

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

#ifndef COVERAGEALIGNMENT_H
#define COVERAGEALIGNMENT_H

// local includes
#include "geoc.h"

// geos includes
#include <geos/operation/distance/DistanceOp.h>
#include <geos/geom/GeometryCollection.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateArraySequenceFactory.h>

// std includes
#include <algorithm>

using namespace geos::operation::distance;

using namespace geoc;
using namespace geoc::geo;
using namespace geoc::alg;
using namespace geoc::edit;
using namespace geoc::tin;

namespace geoc {
namespace alg {

/** Class for coverage alignment of two vector layers. */

class CoverageAlignment
{

public:

    /** Default constructor.
      */
    CoverageAlignment();

    /** Constructor.
      */
    CoverageAlignment( TGeomLayer &ref, TGeomLayer &sub, double tol = 0);

    /** Destructor.
      */
    ~CoverageAlignment();

    /** Set reference layer.
      */
    void setRefGeometry( TGeomLayer &ref ){ refLayer = ref; }

    /** Set subject layer.
      */
    void setSubGeometry( TGeomLayer &sub ){ subLayer = sub; }

    /** Set tolerance distance.
      */
    void setTolDistance( double tol ){ tolDistance = tol; }

    /** Set if geometry correction is wanted. */
    void setRepair( bool repair ){ correct = repair; }

    /** Align two layers. */
    void align();

    /** Get new layer.
      * @return New layer as TGeomLayer.
      */
    TGeomLayer & getNewGeometry() { return newLayer; }

    /** Get list of invalid geometries.
      */
    vector<int> getInvalidGeometries() { return invalids; }


private:

    TGeomLayer refLayer;
    TGeomLayer subLayer;
    TGeomLayer newLayer;
    CoordinateSequence *matchingPoints;
    CoordinateSequence *matchingPointsRef;
    TTin *ttin;
    vector<int> invalids;
    double tolDistance;
    int found;
    bool correct;

    /** Find matching features in ref and sub layer.
      */
    void findMatchingFeatures();

    /** Choose matching points for triangulation.
      */
    void chooseMatchingPoints();

    /** Find closest points from given CoordinateSequences and add them to the matchingPoints (tin vertices).
      * @param g1 Geometry of new layer.
      * @param g2 Geometry of reference layer.
      */
    void findClosestPoints( const Geometry *g1, const Geometry *g2 );

    /** Remove repeated points from matchingPoints and matchingPointsRef.
      */
    void cleanMatchingPoints();

    /** Add corner points to be sure that tin covers all the matching features.
      * @param vector with all points from matching features
      */
    void addCornerPoints( vector<Coordinate>& vc );

    /** Delete repeated points in given vector and also corresponding points from matching points.
      * If given vector is vector with points from matchingPoints, deletes repeated points from it
      * and its equivalents from matchingPointsRef.
      * @param vc Vector with points from matchingPoints or matchingPointsRef
      */
    void deleteRepeated( vector<Coordinate> & vc);

    /** Create TIN from matching points of subject/new layer
      */
    void createTIN();

    /** Find corresponding points to tin ( tin points from refLayer)
      * @param c CoordinateSequence with tin triangle points.
      * @param c2 Searched CoordinateSequence corresponding to ttin with points from reference layer.
      */
    void correspondingPoints( const CoordinateSequence * c, CoordinateSequence *c2 );

    /** Transform new geometry.
      */
    void transform();

};


} // namespace geoc
} // namespace alg

#endif // COVERAGEALIGNMENT_H
