#include "triangle.h"

Triangle::Triangle()
{
    triangle = NULL;
    correspondingT = NULL;
    triangleGeom = NULL;
}

Triangle::~Triangle()
{
    /*if (triangle) delete triangle;
    if (correspondingT) delete correspondingT;
    if( triangleGeom) delete triangleGeom;*/
}


bool Triangle::isInside( const Coordinate *point ) const
{
    GeometryFactory f;
      // memory error
    Geometry *p = f.createPoint( *point);       // memory error

    bool inside = triangleGeom->intersects( p );

    //f.destroyGeometry(g);
    //f.destroyGeometry(p);

    return inside;

} // bool Triangle::isInside( const Coordinate *point ) const

