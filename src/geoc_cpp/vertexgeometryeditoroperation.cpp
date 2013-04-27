/***************************************************************************
    vertexgeometryeditoroperation.cpp

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

#include "vertexgeometryeditoroperation.h"

namespace geoc {
namespace edit {

CoordinateSequence* VertexGeometryEditorOperation::edit(const CoordinateSequence *, const Geometry *geom )
{
    CoordinateSequence* coord = geom->getCoordinates();

    // find closest point from closeCoord
    for ( size_t i = 0; i < coord->getSize(); i++)
    {
        // minimal distance
        double minDist = tolDistance;
        size_t indMin = 0;
        bool isMin = false;

        for ( size_t j = 0; j < closeCoord->getSize(); j++ )
        {
            // compute distance between two tested points
            double dist = coord->getAt(i).distance( closeCoord->getAt(j) );

            if( (0 < dist) && (dist < minDist) )
            {
                minDist = dist;
                indMin = j;
                isMin = true;
            }

        }

        // set new coordinate to the closest point if there is some
        if ( isMin )
        {
            coord->setAt( closeCoord->getAt(indMin), i );
            changed = true;
        }

    }

    return coord;

}

} //namespace geoc
} //namespace edit
