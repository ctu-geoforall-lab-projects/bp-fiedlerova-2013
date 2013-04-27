/***************************************************************************
    spatialindexbuilder.h

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
