/***************************************************************************
    triangulation.h

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

#ifndef TRIANGULATION_H
#define TRIANGULATION_H

// local includes
#include "geoc.h"

// geos includes
#include <geos/triangulate/DelaunayTriangulationBuilder.h>
#include <geos/geom/GeometryCollection.h>

using namespace geos;
using namespace triangulate;

using namespace geoc;
using namespace geoc::geo;
using namespace geoc::alg;

namespace geoc {
namespace tin {

/** Class for Delaunay triangulation */


class Triangulation
{

public:
    /** Default constructor
      */
    Triangulation(): vertices(NULL) {}

    /** Constructor
      */
    Triangulation( CoordinateSequence *c ): vertices(c) {}

    /** Set vertices of triangulation
      * @param points CoordinateSequence with vertices of triangulation
      */
    void setTINVertices( CoordinateSequence *points );

    /** Get triangles.
      * @return GeometryCollection of triangles represented as polygons.
      */
    GeometryCollection* getTriangles();

private:

    CoordinateSequence * vertices;
    DelaunayTriangulationBuilder tBuilder;

};

} //namespace geoc
} //namespace tin


#endif // TRIANGULATION_H
