#include "conflategeometryeditoroperation.h"

#include<iostream>

CoordinateSequence* ConflateGeometryEditorOperation::edit(const CoordinateSequence *, const Geometry *g )
{
    CoordinateSequence* coord = g->getCoordinates();
    size_t cSize = coord->size();

    // transform each point from geometry
    for ( size_t i = 0; i < cSize; i++ )
    {
        Coordinate *point = new Coordinate( coord->getX(i), coord->getY(i) );

        if ( findIdPoints( point ) )
        {
            // set identical points and transform point
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

} // CoordinateSequence* ConflateGeometryEditorOperation::edit(const CoordinateSequence *, const Geometry *g )


bool ConflateGeometryEditorOperation::findIdPoints( Coordinate *point )
{
    // test all triangles
    size_t tSize = tin->size();
    for ( size_t i = 0; i < tSize; i++ )
    {
        // check if point is inside triangle
        if ( tin->at(i).isInside( point ) )
        {
            idPoints1 = tin->at(i).getTriangle();
            idPoints2 = tin->at(i).getCorrespondingTriangle();

            return true;
        }

    }

    return false;

} // bool ConflateGeometryEditorOperation::findIdPoints( Coordinate *point )
