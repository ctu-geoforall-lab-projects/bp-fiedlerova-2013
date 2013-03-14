#include "triangulation.h"

void Triangulation::setTINVertices( CoordinateSequence *points )
{
    tBuilder.setSites(*points);

} // void Triangulation::setTINVertices( CoordinateSequence *points )


std::auto_ptr<GeometryCollection> Triangulation::getTriangles()
{
    GeometryFactory fact;
    std::auto_ptr<GeometryCollection> gColl = tBuilder.getTriangles( fact );

    return gColl;
}
