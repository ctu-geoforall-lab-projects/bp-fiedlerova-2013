#ifndef TRIANGULATION_H
#define TRIANGULATION_H

// local includes
#include "geoc.h"

// geos includes
#include <geos/triangulate/DelaunayTriangulationBuilder.h>

using namespace geos;
using namespace triangulate;

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
    std::auto_ptr<GeometryCollection> getTriangles();

private:
    CoordinateSequence * vertices;
    DelaunayTriangulationBuilder tBuilder;

};

#endif // TRIANGULATION_H
