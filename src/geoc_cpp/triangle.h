#ifndef TRIANGLE_H
#define TRIANGLE_H

// GEOS includes
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/LinearRing.h>
#include <geos/geom/Point.h>

using namespace std;
using namespace geos;
using namespace geos::geom;

/** Class representing TIN triangle */

class Triangle
{

public:

    /** Constructor
      */
    Triangle();

    /** Set triangle
      */
    void setTriangle( CoordinateSequence *t ){ triangle = t; }

    /** Set corresponding triangle
      */
    void setCorrespondingTriangle( CoordinateSequence *t ){ correspondingT = t; }

    /** Get triangle
      */
    CoordinateSequence* getTriangle(){ return triangle; }

    /** Get corresponding triangle
      */
    CoordinateSequence* getCorrespondingTriangle(){ return correspondingT; }

    /** Test whether point is inside triangle.
      * @param point Tested point.
      * @return true if point is inside this triangle.
      */
    bool isInside( const Coordinate *point ) const;

private:
    CoordinateSequence* triangle;
    CoordinateSequence *correspondingT;

};

typedef vector< Triangle > TTin;

#endif // TRIANGLE_H
