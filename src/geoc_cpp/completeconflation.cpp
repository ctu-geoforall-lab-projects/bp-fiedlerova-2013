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
   if (matchingPoints) delete matchingPoints;
   if (matchingPointsRef) delete matchingPointsRef;
   if (ttin) delete ttin;

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

            findClosestPoints( g1, g2 );
        }
    }

} // void CompleteConflation::chooseMatchingPoints()


void CompleteConflation::findClosestPoints( const Geometry *g1, const Geometry *g2 )
{
    CoordinateSequence *c1 = g1->getCoordinates();
    CoordinateSequence *c2 = g2->getCoordinates();

    // sort coordinates -> for controlling repeated points
    sortCoordinates(c1);
    sortCoordinates(c2);

    // find closest pairs of points
    for ( size_t i = 0; i < c1->size(); i++)
    {
        // minimal distance
        double minDist = tolDistance;
        size_t indMin = 0;
        bool isMin = false;

        for ( size_t j = 0; j < c2->size(); j++ )
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

            // check added points and remove repeated if necessary
            removeRepeatedPoints();
        }

    }

    delete c1;
    delete c2;


} // void CompleteConflation::findClosestPoints( const Geometry *g1, const Geometry *g2 )


void CompleteConflation::sortCoordinates( CoordinateSequence * c )
{
    // transfer to vector
    vector<Coordinate> vc;
    c->toVector( vc );

    // sort
    sort( vc.begin(), vc.end() );

    // set sorted points to c
    c->setPoints(vc);

} // void CompleteConflation::sortCoordinates( CoordinateSequence * c )


void CompleteConflation::removeRepeatedPoints()
{

    size_t mP = matchingPoints->size();
    size_t mPRef = matchingPointsRef->size();

    // not allow repeated points to the one
    if( mP == mPRef+1 )
    {
        // delete the farther point from matchingPoints
        if ( matchingPoints->getAt(mP-1).distance( matchingPointsRef->getAt(mPRef-1)) >
             matchingPoints->getAt(mP-2).distance( matchingPointsRef->getAt(mPRef-1)))
        {
            matchingPoints->deleteAt(mP-1);
        }
        else
        {
            matchingPoints->deleteAt(mP-2);
        }

    }
    else if ( mP == mPRef-1 )
    {
        // delete the farther point from matchingPointsRef
        if ( matchingPointsRef->getAt(mPRef-1).distance( matchingPoints->getAt(mP-1)) >
             matchingPointsRef->getAt(mPRef-2).distance( matchingPoints->getAt(mP-1)))
        {
            matchingPointsRef->deleteAt(mPRef-1);
        }
        else
        {
            matchingPointsRef->deleteAt(mPRef-2);
        }

    }

} // void CompleteConflation::removeRepeatedPoints()


void CompleteConflation::createTIN()
{

    ttin = new TTin();

    GeometryFactory gf;

    // initialize tin maker
    Triangulation tinMaker1;
    tinMaker1.setTINVertices( matchingPoints );

    // create tin
    GeometryCollection* tin1 = tinMaker1.getTriangles();

    // transfer tins to Ttin
    size_t tSize = tin1->getNumGeometries(); // tin from matchinPoints


    for ( size_t n = 0; n < tSize; n++ )
    {
        Polygon *t1 = dynamic_cast<Polygon*>( tin1->getGeometryN(n)->clone() );

        Geometry * extRing = t1->getExteriorRing()->clone();
        CoordinateSequence *c1 = extRing->getCoordinates();

        // find corresponding triangle
        CoordinateSequence *c2 = new CoordinateArraySequence();
        correspondingPoints( c1, c2 );

        // set triangle
        Triangle triangle;
        triangle.setTriangleGeom( extRing );
        triangle.setTriangle( c1 );
        triangle.setCorrespondingTriangle( c2 );

        ttin->push_back( triangle );

        gf.destroyGeometry(t1);

    }

    // for clear memory
    gf.destroyGeometry(tin1);

} // void CompleteConflation::createTIN()


void CompleteConflation::correspondingPoints( const CoordinateSequence * c, CoordinateSequence * c2 )
{
    size_t cSize = c->size();

    // transfer coordinates to vector
    vector<Coordinate> vc;
    matchingPoints->toVector(vc);

    // find corresponding points to tin points in matchingPointsRef
    for ( size_t i = 0; i < cSize; i++ )
    {
        vector<Coordinate>::iterator it = find( vc.begin(), vc.end(), c->getAt(i));
        if ( it != vc.end() )
        {
            c2->add( matchingPointsRef->getAt( it - vc.begin()) );
        }
    }

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
} // void CompleteConflation::transform()


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
