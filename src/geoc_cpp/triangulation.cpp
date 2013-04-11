#include "triangulation.h"

namespace geoc {
namespace tin {

void Triangulation::setTINVertices( CoordinateSequence *points )
{
    tBuilder.setSites(*points);

} // void Triangulation::setTINVertices( CoordinateSequence *points )


GeometryCollection* Triangulation::getTriangles()
{
    GeometryFactory fact;
    std::auto_ptr<GeometryCollection> gColl = tBuilder.getTriangles( fact );

    return gColl.release();
}

} //namespace geoc
} //namespace tin
