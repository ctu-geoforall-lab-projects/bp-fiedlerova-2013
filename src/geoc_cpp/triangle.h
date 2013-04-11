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

namespace geoc {
namespace tin {

/** Class representing TIN triangle */

class Triangle
{

public:

    /** Constructor
      */
    Triangle();

    /** Destructor
      */
    ~Triangle();

    /** Set triangle
      */
    void setTriangle( CoordinateSequence *t ){ triangle = t; }

    /** Set corresponding triangle
      */
    void setCorrespondingTriangle( CoordinateSequence *t ){ correspondingT = t; }

    /** Get triangle
      */
    CoordinateSequence* getTriangle(){ return triangle; }

    /** Set triangle geometry
      */
    void setTriangleGeom( Geometry *g ){ triangleGeom = g; }

    /** Get geometry of triangle
      */
    Geometry * getTriangleGeom(){ return triangleGeom; }

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
    Geometry *triangleGeom;

};

typedef vector< Triangle > TTin;


} //namespace geoc
} //namespace tin

#endif // TRIANGLE_H
