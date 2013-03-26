#include "completeconflation.h"

#include <iostream>

CompleteConflation::CompleteConflation()
{
    tolDistance = 0;
    matchingPoints = NULL;
    matchingPointsRef = NULL;
    ttin = NULL;

} // default constructor


CompleteConflation::CompleteConflation( TGeomLayer &ref, TGeomLayer &sub, double tol )
{
    refLayer = ref;
    subLayer = sub;
    tolDistance = tol;
    matchingPoints = NULL;
    matchingPointsRef = NULL;
    ttin = NULL;

} // constructor


CompleteConflation::~CompleteConflation()
{
   // delete matchingPoints;
   // delete matchingPointsRef;
    //delete ttin;

} // destructor


void CompleteConflation::findMatchingFeatures()
{
    // create new matcher
    MatchingGeometry matcher(&refLayer, tolDistance);

    // copy geometries from subject layer to the new one
    newLayer = subLayer;

    // set matching feature to each feature from new layer
    size_t size = newLayer.size();
    for ( size_t i = 0; i < size; i++ )
    {
        matcher.setMatch( &newLayer[i] );
    }

} // void CompleteConflation::findMatchingFeatures()


void CompleteConflation::chooseMatchingPoints()
{
    matchingPoints = new CoordinateArraySequence();
    matchingPointsRef = new CoordinateArraySequence();

    // find pairs of close points from matching geometries in reference and new layer
    size_t nSize = newLayer.size();
    for ( size_t i = 0; i < nSize; i++ )
    {
        // closest point only if geometry has a matching one
        if ( newLayer[i].isMatch() )
        {
            Geometry* g1 = newLayer[i].getGEOSGeom();
            Geometry* g2 = newLayer[i].getMatched();

            if (!g2->isEmpty() && g2->isValid())
            {
                findClosestPoints( g1, g2 );
            }
        }
    }

} // void CompleteConflation::chooseMatchingPoints()


void CompleteConflation::findClosestPoints( const Geometry *g1, const Geometry *g2 )
{
    CoordinateSequence *c1 = g1->getCoordinates();
    CoordinateSequence *c2 = g2->getCoordinates();

    // find pairs of closest points
   /* CoordinateSequence *nearest = DistanceOp::nearestPoints( g1, g2 );

    const Coordinate c1 = nearest->getAt(0);
    const Coordinate c2 = nearest->getAt(1);

    if ( c1.distance(c2) <= tolDistance )
    {
        qDebug("is match");
        matchingPoints->add( c1, false );
        matchingPointsRef->add( c2, false );
    } */

    // find closest pair of points
    for ( size_t i = 0; i < c1->getSize(); i++)
    {
        // minimal distance
        double minDist = tolDistance;
        size_t indMin = 0;
        bool isMin = false;

        for ( size_t j = 0; j < c2->getSize(); j++ )
        {
            // compute distance between two tested points
            double dist = c1->getAt(i).distance( c2->getAt(j) );

            if( dist < minDist )
            {
                minDist = dist;
                indMin = j;
                isMin = true;
            }

        }

        // set closest pair of points
        if ( isMin )
        {
            matchingPoints->add( c1->getAt(i), false );
            matchingPointsRef->add( c2->getAt(indMin), false );

            size_t mP = matchingPoints->size();
            size_t mPRef = matchingPointsRef->size();

            // not allow more corresponding point to the one
            if( mP == mPRef+1 )
            {
                matchingPoints->deleteAt(mP-1);
            }
            else if ( mP == mPRef-1 )
            {
                matchingPointsRef->deleteAt(mPRef-1);
            }

        }

    }

} // void CompleteConflation::findClosestPoints( CoordinateSequence *c1, CoordinateSequence *c2 )


void CompleteConflation::createTIN()
{

    ttin = new TTin();

    // initialize tin maker
    Triangulation tinMaker1;
    tinMaker1.setTINVertices( matchingPoints );

    // create tin
    GeometryCollection* tin1 = tinMaker1.getTriangles();

    // transfer tins to Ttin
    size_t tSize = tin1->getNumGeometries(); // tin from matchinPoints


    for ( size_t n = 0; n < tSize; n++ )
    {
        Polygon *t1 = dynamic_cast<Polygon*>(tin1->getGeometryN(n)->clone());

        CoordinateSequence *c1 = t1->getExteriorRing()->getCoordinates();

        // find corresponding triangle
        CoordinateSequence * c2 = correspondingPoints(c1);

        // set triangle
        Triangle triangle;
        triangle.setTriangle( c1 );
        triangle.setCorrespondingTriangle( c2 );

        ttin->push_back( triangle );

    }


} // void CompleteConflation::createTIN()


CoordinateSequence * CompleteConflation::correspondingPoints( const CoordinateSequence * c )
{
    CoordinateSequence *c2 = new CoordinateArraySequence();

    size_t mSize = matchingPoints->size();
    size_t cSize = c->size();

    // find equivalent to each c point and add it to c2
    for ( size_t i = 0; i < cSize; i++ )
    {
        for ( size_t j = 0; j < mSize; j++ )
        {
            if ( c->getAt(i).equals( matchingPoints->getAt(j) ) )
            {
                c2->add( matchingPointsRef->getAt(j) );

            }
        }
    }

    return c2;

} // CoordinateSequence * CompleteConflation::coresspondingPoints( const CoordinateSequence * c )


void CompleteConflation::transform()
{
    // create and set geometry editor
    ConflateGeometryEditorOperation myOp;
    myOp.setTIN( ttin );

    size_t nSize = newLayer.size();
    for ( size_t i = 0; i < nSize; i++ )
    {
        Geometry *geom = newLayer[i].getGEOSGeom();
        GeometryEditor geomEdit( geom->getFactory() );

        // set geometry to edited one
        newLayer[i].setGEOSGeom( geomEdit.edit( geom , &myOp ) );

        // check if geometry was changed
        newLayer[i].setChanged( myOp.isChanged() );

        // check validity
        if( !newLayer[i].getGEOSGeom()->isValid() )
        {
            qDebug("CompleteConflation::transform: Geom is not valid.");
            invalids.push_back( newLayer[i].getFeatureId() );
        }

    }
}

void CompleteConflation::conflate()
{
    qDebug("CompleteConflation::conflate: Entering.");

    // find matching points
    findMatchingFeatures();
    qDebug("CompleteConflation::conflate: Matching feature done.");

    chooseMatchingPoints();
    qDebug("CompleteConflation::conflate: Matching done.");

    // create TIN
    createTIN();
    qDebug("CompleteConflation::conflate: TIN done.");

    // transform geometry
    transform();
    qDebug("CompleteConflation::conflate: Transform done.");

} // void CompleteConflation::conflate()
