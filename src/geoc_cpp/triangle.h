/***************************************************************************
    triangle.h

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
    Triangle(): triangle(NULL), correspondingT(NULL), triangleGeom(NULL){}

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

private:

    CoordinateSequence* triangle;
    CoordinateSequence *correspondingT;
    Geometry *triangleGeom;

};

typedef vector< Triangle > TTin;


} //namespace geoc
} //namespace tin

#endif // TRIANGLE_H
