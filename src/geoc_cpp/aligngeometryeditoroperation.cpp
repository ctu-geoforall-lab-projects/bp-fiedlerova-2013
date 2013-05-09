/***************************************************************************
    aligngeometryeditoroperation.cpp

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

#include "aligngeometryeditoroperation.h"

namespace geoc {
namespace edit {

AlignGeometryEditorOperation::~AlignGeometryEditorOperation()
{
    if (sIndex)
    {
        delete sIndex;
    }
} // constructor


void AlignGeometryEditorOperation::setTIN(TTin *t)
{
    ttin = t;

#ifdef WITHOUT_SPIDX
#else
    sIndex = SpatialIndexBuilder::buildIndex(ttin);
#endif

} // void AlignGeometryEditorOperation::setTIN(TTin *t)

CoordinateSequence* AlignGeometryEditorOperation::edit(const CoordinateSequence *, const Geometry *g )
{
    CoordinateSequence* coord = g->getCoordinates();
    size_t cSize = coord->size();

    // transform each point from geometry
    for ( size_t i = 0; i < cSize; i++ )
    {
        Coordinate *point = new Coordinate( coord->getX(i), coord->getY(i) );

        if ( findIdPoints( point ) )
        {
            AffineTransformation at;
            at.setIdenticPoints1( idPoints1 );
            at.setIdenticPoints2( idPoints2 );
            at.transformPoint2D( point );
            changed = true;
        }

        coord->setAt( *point, i );

        delete point;

    }

    return coord;

} // CoordinateSequence* AlignGeometryEditorOperation::edit(const CoordinateSequence *, const Geometry *g )

#ifdef WITHOUT_SPIDX  // for testing

bool AlignGeometryEditorOperation::findIdPoints( Coordinate *point )
{

    // build point geometry
    GeometryFactory f;
    Geometry * p = f.createPoint( *point );

    // test all close triangles
    for ( size_t i = 0; i < ttin->size(); i++ )
    {

        Triangle t = ttin->at(i);
        Geometry *g = f.createGeometry(t.getTriangleGeom());


        // check if point is inside triangle
        if ( g->intersects( p ) )
        {
            idPoints1 = t.getTriangle();
            idPoints2 = t.getCorrespondingTriangle();

            f.destroyGeometry(p);
            f.destroyGeometry(g);
            return true;
        }

        f.destroyGeometry(g);

    }

    f.destroyGeometry(p);
    return false;

} // bool AlignGeometryEditorOperation::findIdPoints( Coordinate *point )

#else

bool AlignGeometryEditorOperation::findIdPoints( Coordinate *point )
{
    // use spatial index
    vector<void*> results;
    Envelope * searchEnv = new Envelope( *point );
    sIndex->query( searchEnv, results );
    size_t rSize = results.size();

    // build point geometry
    GeometryFactory f;
    Geometry * p = f.createPoint( *point );

    // test all close triangles
    for ( size_t i = 0; i < rSize; i++ )
    {
        Geometry *searchGeom = static_cast<Geometry*>( results[i] );
        Geometry *g = f.createGeometry(searchGeom);
        Triangle * t = static_cast<Triangle *>( searchGeom->getUserData() );

        // check if point is inside triangle
        if ( g->intersects( p ) )
        {
            idPoints1 = t->getTriangle();
            idPoints2 = t->getCorrespondingTriangle();

            delete searchEnv;
            f.destroyGeometry(p);
            f.destroyGeometry(g);
            return true;
        }

        f.destroyGeometry(g);

    }

    delete searchEnv;
    f.destroyGeometry(p);
    return false;

} // bool AlignGeometryEditorOperation::findIdPoints( Coordinate *point )

#endif


} //namespace geoc
} //namespace edit
