#ifndef COMPLETECONFLATION_H
#define COMPLETECONFLATION_H

// local includes
#include "geoc.h"

// geos includes
#include <geos/operation/distance/DistanceOp.h>
#include <geos/geom/GeometryCollection.h>
#include <geos/geom/Polygon.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateArraySequenceFactory.h>

using namespace geos::operation::distance;

/** Class for vector conflation of two layers. */

class CompleteConflation
{

public:

    /** Default constructor.
      */
    CompleteConflation();

    /** Constructor.
      */
    CompleteConflation( TGeomLayer &ref, TGeomLayer &sub, double tol = 0);

    /** Destructor.
      */
    ~CompleteConflation();

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

    /** Create TIN from matching points of subject/new layer
      */
    void createTIN();

    /** Find corresponding points to tin ( tin points from refLayer)
      * @return CoordinateSequence corresponding to ttin with points from reference layer.
      */
    CoordinateSequence * correspondingPoints( const CoordinateSequence * c );

    /** Transform new geometry.
      */
    void transform();

    /** Conflate two layers. */
    void conflate();

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

};

#endif // COMPLETECONFLATION_H
