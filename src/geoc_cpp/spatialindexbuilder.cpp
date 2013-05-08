/***************************************************************************
    spatialindexbuilder.cpp

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

#include "spatialindexbuilder.h"

namespace geoc {
namespace idx {

SpatialIndexBuilder::SpatialIndexBuilder()
{
}

SpatialIndex * SpatialIndexBuilder::buildIndex( TGeomLayer * layer)
{
    // create new index

    SpatialIndex *sIndex = new STRtree();

    // add envelopes of geometries to index
    size_t gSize = layer->size();
    for ( size_t i = 0; i < gSize; i++ )
    {
        const Geometry* g = layer->at(i).getGEOSGeom();
        const Envelope* env = g->getEnvelopeInternal();
        sIndex->insert(env, (void*)g );
    }

    return sIndex;

} // SpatialIndex * SpatialIndexBuilder::buildIndex( TGeomLayer * layer)


SpatialIndex *SpatialIndexBuilder::buildIndex( TTin * t )
{
    // create new index
    SpatialIndex *sIndex = new STRtree();

    // add envelopes of geometries to index
    size_t gSize = t->size();
    for ( size_t i = 0; i < gSize; i++ )
    {
        Geometry* g = t->at(i).getTriangleGeom();
        g->setUserData( (void*)(&t->at(i)) );
        const Envelope* env = g->getEnvelopeInternal();

        sIndex->insert(env, (void*)g );
    }

    return sIndex;

} // SpatialIndex *SpatialIndexBuilder::buildIndex( TTin * t )

} // namespace geoc
} // namespace idx
