#include "completeconflation.h"

void CompleteConflation::findMatchingFeatures()
{
    // create new matcher
    MatchingGeometry matcher(refLayer, tolDistance);

    // copy geometries from subject layer to the new one
    newLayer = *subLayer;

    // set matching feature to each feature from new layer
    unsigned int size = newLayer.size();
    for ( unsigned int i = 0; i < size; i++ )
    {
        matcher.setMatch( &newLayer[i] );
    }

} // void CompleteConflation::findMatchingFeatures()


void CompleteConflation::chooseMatchingPoints()
{

    //matchingPoints,Ref = resultOfThisFunction;

} // void CompleteConflation::chooseMatchingPoints()


void CompleteConflation::createTIN()
{
    // initialize tin maker
    Triangulation tinMaker;
    tinMaker.setTINVertices( matchingPoints );

    // create tin
    tin = (tinMaker.getTriangles()).get();


    // IS THIS NECCESSARY?????????????
    // initialize tin maker
    Triangulation tinMaker2;
    tinMaker2.setTINVertices( matchingPointsRef );

    // create tin
    tinRef = (tinMaker2.getTriangles()).get();

} // void CompleteConflation::createTIN()


bool CompleteConflation::isInside( const Coordinate *point, const CoordinateSequence *triangle  ) const
{
    GeometryFactory f;
    Geometry *g = f.createLinearRing( triangle->clone() );

    return g->contains( f.createPoint( *point) );

} // bool CompleteConflation::isInside( const Coordinate *point, const Polygon *triangle  ) const


CoordinateSequence * CompleteConflation::findPointsToTransform( const CoordinateSequence *triangle )
{
    // intialize coord. sequence
    CoordinateSequence *insidePoints = new CoordinateArraySequence();

    // find all points inside triangle
    unsigned int size = newLayer.size();
    for ( unsigned int i = 0; i < size; i++)
    {
        // get points from geometry
        CoordinateSequence *points = newLayer[i].getGEOSGeom()->getCoordinates();
        unsigned int pSize = points->size();

        // test whether point is inside triangle
        for ( unsigned int j = 0; j < pSize; j++ )
        {
            if( isInside( &(points->getAt(j)), triangle ) )
            {
                insidePoints->add( points->getAt(j) );
            }
        }
    }

    return insidePoints;

} // CoordinateSequence * CompleteConflation::findPointsToTransform( const Polygon *triangle )


void CompleteConflation::affineTransform( const CoordinateSequence &identicPoints1, const CoordinateSequence &identicPoints2 )
{

    // affine transformation provider
    AffineTransformation affTransformator;
    affTransformator.setIdenticPoints1( &identicPoints1 );
    affTransformator.setIdenticPoints2( &identicPoints2 );

    // find points inside triangle
    CoordinateSequence *transfSet = findPointsToTransform( &identicPoints2 );
    affTransformator.setTransfSet( transfSet );

    // transform points inside triangle
    affTransformator.affineTransf2D();

} // void CompleteConflation::affineTransform(const CoordinateSequence &identicPoints1, const CoordinateSequence &identicPoints2)


void CompleteConflation::conflate()
{

    // find matching points
    findMatchingFeatures();
    chooseMatchingPoints();

    // create TIN
    createTIN();

    // transform points in each triangle
    unsigned int tSize = tin->getNumGeometries(); // tin from matchinPoints

    for ( unsigned int n = 0; n < tSize; n++ )
    {
        CoordinateSequence *ipoints2 = tin->getGeometryN(n)->getCoordinates();
        CoordinateSequence *ipoints1 = tinRef->getGeometryN(n)->getCoordinates();

        affineTransform( *ipoints1, *ipoints2 );
    }

} // void CompleteConflation::conflate()
