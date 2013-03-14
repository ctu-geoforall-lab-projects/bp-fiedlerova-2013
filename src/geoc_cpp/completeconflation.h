#ifndef COMPLETECONFLATION_H
#define COMPLETECONFLATION_H

// local includes
#include "geoc.h"

/** Class for vector conflation of two layers */

class CompleteConflation
{

public:

    /** Default constructor
      */
    CompleteConflation(): refLayer(NULL), subLayer(NULL), tolDistance(0), matchingPoints(NULL), matchingPointsRef(NULL), tin(NULL), tinRef(NULL){}

    /** Constructor
      */
    CompleteConflation( TGeomLayer *ref, TGeomLayer *sub, double tol = 0): refLayer(ref), subLayer(sub), tolDistance(tol), matchingPoints(NULL), matchingPointsRef(NULL), tin(NULL), tinRef(NULL){}

    /** Set reference layer */
    void setRefLayer( TGeomLayer *ref ){ refLayer = ref; }

    /** Set subject layer */
    void setSubLayer( TGeomLayer *sub ){ subLayer = sub; }

    /** Set tolerance distance */
    void setTolDistance( double tol ){ tolDistance = tol; }

    /** Find matching features in ref and sub layer*/
    void findMatchingFeatures();

    /** Choose matching points for triangulation */
    void chooseMatchingPoints();

    /** Create TIN */
    void createTIN();

    /** Find points to be transformed */
    CoordinateSequence * findPointsToTransform( const CoordinateSequence *triangle );

    /** Test whether point is inside triangle */
    bool isInside( const Coordinate *point, const CoordinateSequence *triangle ) const;

    /** Transform points in geometries - local affine transformation
      * @param identicPoints1 Points from reference triangle.
      * @param identicPoints2 Points from subject triangle. */
    void affineTransform( const CoordinateSequence &identicPoints1, const CoordinateSequence &identicPoints2 );

    /** Conflate two layers */
    void conflate();

    /** Get new layer
      * @return New layer as TGeomLayer
      */
    TGeomLayer & getNewLayer() { return newLayer; }

private:

    TGeomLayer *refLayer;
    TGeomLayer *subLayer;
    TGeomLayer newLayer;
    double tolDistance;
    CoordinateSequence *matchingPoints;
    CoordinateSequence *matchingPointsRef;
    GeometryCollection *tin;
    GeometryCollection *tinRef;

};

#endif // COMPLETECONFLATION_H
