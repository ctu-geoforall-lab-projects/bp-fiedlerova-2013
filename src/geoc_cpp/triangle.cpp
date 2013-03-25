#include "triangle.h"

Triangle::Triangle()
{
}


bool Triangle::isInside( const Coordinate *point ) const
{
    GeometryFactory f;
    Geometry *g = f.createLinearRing( triangle );

    return g->contains( f.createPoint( *point) );

} // bool Triangle::isInside( const Coordinate *point ) const

