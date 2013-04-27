/***************************************************************************
    triangulation.cpp

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
