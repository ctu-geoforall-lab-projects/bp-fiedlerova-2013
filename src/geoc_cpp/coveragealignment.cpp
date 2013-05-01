/***************************************************************************
    coveragealignment.cpp

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

#include "coveragealignment.h"

namespace geoc {
namespace alg {

CoverageAlignment::CoverageAlignment()
{
    matchingPoints = NULL;
    matchingPointsRef = NULL;
    ttin = NULL;
    tolDistance = 0;
    found = 0;

} // default constructor


CoverageAlignment::CoverageAlignment( TGeomLayer &ref, TGeomLayer &sub, double tol)
{
    refLayer = ref;
    subLayer = sub;
    matchingPoints = NULL;
    matchingPointsRef = NULL;
    ttin = NULL;
    tolDistance = tol;
    found = 0;
    correct = false;

} // constructor


CoverageAlignment::~CoverageAlignment()
{

} // destructor


void CoverageAlignment::findMatchingFeatures()
{

    // create new matcher
    MatchingGeometry matcher(&refLayer, tolDistance);

    // copy geometries from subject layer to the new one
    newLayer = subLayer;

    // set matching feature to each feature from new layer
    size_t size = newLayer.size();
    for ( size_t i = 0; i < size; i++ )
    {

        if ( !newLayer[i].isMatch() )
        {
            matcher.setMatch( &newLayer[i] );
            if( newLayer[i].isMatch() )
            {
                found++;
            }
        }

    }

} // void CoverageAlignment::findMatchingFeatures()


void CoverageAlignment::chooseMatchingPoints()
{

    matchingPoints = new CoordinateArraySequence();
    matchingPointsRef = new CoordinateArraySequence();

    CoordinateSequence *mfPoints = new CoordinateArraySequence();
    vector<Coordinate> vc;

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

            mfPoints->add( g1->getCoordinates(), false, true );

        }
    }

    cleanMatchingPoints();

    // add extreme points for tin
    mfPoints->toVector( vc );
    addCornerPoints( vc );

    delete mfPoints;

} // void CoverageAlignment::chooseMatchingPoints()


void CoverageAlignment::findClosestPoints( const Geometry *g1, const Geometry *g2 )
{

    CoordinateSequence *c1 = g1->getCoordinates();
    CoordinateSequence *c2 = g2->getCoordinates();

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

            if( (0 < dist) && (dist < minDist) )
            {
                minDist = dist;
                indMin = j;
                isMin = true;
            }

        }

        // set closest pair of points
        if ( isMin )
        {
            matchingPoints->add( c1->getAt(i), true );  // NOTE: repeated points will be removed later
            matchingPointsRef->add( c2->getAt(indMin), true );
        }

    }

    delete c1;
    delete c2;


} // void CoverageAlignment::findClosestPoints( const Geometry *g1, const Geometry *g2 )


void CoverageAlignment::cleanMatchingPoints()
{

    // transfer to vector
    vector<Coordinate> vc1;
    matchingPoints->toVector( vc1 );

    // delete repeated
    deleteRepeated( vc1 );

    // transfer to vector
    vector<Coordinate> vc2;
    matchingPointsRef->toVector( vc2 );

    // delete repeated
    deleteRepeated( vc2 );


} //void CoverageAlignment::cleanMatchingPoints()


void CoverageAlignment::addCornerPoints( vector<Coordinate>& vc )
{

    // don't add point if there is no reason
    if ( matchingPoints->size() < 1 )
    {
        return;
    }

    // find max and min coordinates
    double maxX = (*max_element( vc.begin(), vc.end(), SortByX() )).x;
    double maxY = (*max_element( vc.begin(), vc.end(), SortByY() )).y;
    double minX = (*min_element( vc.begin(), vc.end(), SortByX() )).x;
    double minY = (*min_element( vc.begin(), vc.end(), SortByY() )).y;

    // corner points
    Coordinate c1, c2, c3, c4;
    c1.x = maxX;
    c1.y = maxY;
    c2.x = minX;
    c2.y = maxY;
    c3.x = minX;
    c3.y = minY;
    c4.x = maxX;
    c4.y = minY;

    // add to matching points
    matchingPoints->add( c1 );
    matchingPoints->add( c2 );
    matchingPoints->add( c3 );
    matchingPoints->add( c4 );
    matchingPointsRef->add( c1 );
    matchingPointsRef->add( c2 );
    matchingPointsRef->add( c3 );
    matchingPointsRef->add( c4 );

} // void CoverageAlignment::addCornerPoints()


void CoverageAlignment::deleteRepeated( vector<Coordinate> & vc)
{

    vector<Coordinate>::iterator it = vc.begin();

    // delete repeated points from matching points
    for ( ; it != vc.end(); it++ )
    {
        vector<Coordinate>::iterator fit = find( it+1, vc.end(),  *it );

        if ( fit != vc.end() )
        {
            size_t fi = fit - vc.begin();
            size_t fj = it - vc.begin();

            // delete the farther pair of points
            if ( matchingPointsRef->getAt(fi).distance( matchingPoints->getAt(fi)) >
                 matchingPointsRef->getAt(fj).distance( matchingPoints->getAt(fj)))
            {
                matchingPoints->deleteAt(fi);
                matchingPointsRef->deleteAt(fi);
                vc.erase(fit);
            }
            else
            {
                matchingPoints->deleteAt(fj);
                matchingPointsRef->deleteAt(fj);
                vc.erase(it);
            }

            it--;

        } // if

    } // for

} // void  CoverageAlignment::deleteRepeated( vector<Coordinate> * vc)


void CoverageAlignment::createTIN()
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

    // set own representation of tin
    for ( size_t n = 0; n < tSize; n++ )
    {
        Polygon *t1 = dynamic_cast<Polygon*>( tin1->getGeometryN(n)->clone() );

        Geometry * pol = t1->clone();
        CoordinateSequence *c1 = t1->getExteriorRing()->getCoordinates();

        // find corresponding triangle
        CoordinateSequence *c2 = new CoordinateArraySequence();
        correspondingPoints( c1, c2 );

        // set triangle
        Triangle triangle;
        triangle.setTriangleGeom( pol );
        triangle.setTriangle( c1 );
        triangle.setCorrespondingTriangle( c2 );

        ttin->push_back( triangle );

        gf.destroyGeometry(t1);

    }

    // for clear memory
    gf.destroyGeometry(tin1);
    if (matchingPoints)
    {
        delete matchingPoints;
    }

    if (matchingPointsRef)
    {
        delete matchingPointsRef;
    }

} // void CoverageAlignment::createTIN()


void CoverageAlignment::correspondingPoints( const CoordinateSequence * c, CoordinateSequence * c2 )
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

} // CoordinateSequence * CoverageAlignment::coresspondingPoints( const CoordinateSequence * c )


void CoverageAlignment::transform()
{
    // TIN has no triangles
    if ( ttin->size() == 0 )
    {
        return;
    }

    // create and set geometry editor
    AlignGeometryEditorOperation myOp;
    myOp.setTIN( ttin );

    invalids.clear();
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
        if (correct)
        {
            GeometryCorrectionOperation::repair(&newLayer[i]);
        }

        // check validity
        if( !newLayer[i].getGEOSGeom()->isValid() )
        {
            invalids.push_back(newLayer[i].getFeatureId());
        }

    }


} // void CoverageAlignment::transform()


void CoverageAlignment::align()
{

    // repeat until no new matches are found
    do
    {
        found = 0;

        // find matching features
        findMatchingFeatures();
        if (found == 0)
        {
            break;
        }

        // find matching points
        chooseMatchingPoints();

        // create TIN
        createTIN();

        // transform geometry
        transform();

        subLayer = newLayer;
        matchingPoints = NULL;
        matchingPointsRef = NULL;
        //if (ttin) delete ttin;
        ttin = NULL;

    } while ( found != 0 );

} // void CoverageAlignment::align()


} // namespace geoc
} // namespace alg
