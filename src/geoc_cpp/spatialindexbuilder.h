#ifndef SPATIALINDEXBUILDER_H
#define SPATIALINDEXBUILDER_H

#include <geos/geom/Envelope.h>
#include <geos/index/strtree/STRtree.h>

// local includes
#include "geoc.h"

using namespace geos::index;
using namespace geos::index::strtree;

using namespace geoc;
using namespace geoc::geo;
using namespace geoc::tin;


namespace geoc {
namespace idx {

/** Class for building spatial index. */

class SpatialIndexBuilder
{

public:

    SpatialIndexBuilder();

    /** Build spatial index on given layer.
      * @param layer Vector with feature geometries.
      * @param sIndex Built index on layer.
      */
    static SpatialIndex * buildIndex( TGeomLayer * layer);

    /** Build spatial index on given tin vector.
      * @param layer Vector with triangles.
      * @param sIndex Built index on tin.
      */
    static SpatialIndex * buildIndex( TTin * t );

};

} // namespace geoc
} // namespace idx

#endif // SPATIALINDEXBUILDER_H
