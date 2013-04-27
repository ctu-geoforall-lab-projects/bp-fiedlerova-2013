/***************************************************************************
    vertexgeometryeditoroperation.h

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

#ifndef VERTEXGEOMETRYEDITOROPERATION_H
#define VERTEXGEOMETRYEDITOROPERATION_H

// GEOS includes
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/util/CoordinateOperation.h>
#include <geos/geom/CoordinateSequenceFactory.h>

// local includes
#include "geoc.h"

#include <QtGui>

using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace geos::geom::util;

using namespace geoc;
using namespace geoc::geo;
using namespace geoc::alg;
using namespace geoc::edit;

namespace geoc {
namespace edit {

/** Class for editing geometry - snap vertices to the closest points. */

class VertexGeometryEditorOperation: public CoordinateOperation
{

    using CoordinateOperation::edit;

public:

    /// Constructor
    VertexGeometryEditorOperation(  ): closeCoord(NULL), tolDistance(0), changed(false){}

    /** Set private closeCoord to given coord.
     */
    void setCloseCoordSeq( CoordinateSequence *coord ) { closeCoord = coord; }

    /** Set tolerance distance
      */
    void setTolDistance( double tol ) { tolDistance = tol; }

    /** Return true if geometry was changed
      */
    bool isChanged() { return changed; }

    /** Virtual function for editing geometry - snap to closest vertices.
     * @param coordinates Not important.
     * @param gGeometry to be edited.
     */
    CoordinateSequence* edit(const CoordinateSequence *coordinates, const Geometry *g );


private:

    CoordinateSequence *closeCoord;
    double tolDistance;
    bool changed;

};

} //namespace geoc
} //namespace edit

#endif // VERTEXGEOMETRYEDITOROPERATION_H
