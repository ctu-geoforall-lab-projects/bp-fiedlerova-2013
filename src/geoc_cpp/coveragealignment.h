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

/** Class for vector conflation of two layers. */

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

    /** Delete repeated points based on given vector.
      */
    void deleteRepeated( vector<Coordinate> & c);

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
    double tolDistance;
    CoordinateSequence *matchingPoints;
    CoordinateSequence *matchingPointsRef;
    TTin *ttin;
    vector<int> invalids;
    int found;

};


} // namespace geoc
} // namespace alg

#endif // COVERAGEALIGNMENT_H
