/***************************************************************************
    vertexsnapper.h

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

#ifndef VERTEXSNAPPER_H
#define VERTEXSNAPPER_H

// GEOS includes
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateArraySequence.h>
#include <geos/geom/util/GeometryEditor.h>
#include <geos/geom/Envelope.h>
#include <geos/index/strtree/STRtree.h>

// std includes
# include <vector>

// local includes
#include "geoc.h"

using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace geos::geom::util;
using namespace geos::index;
using namespace geos::index::strtree;

using namespace geoc;
using namespace geoc::geo;
using namespace geoc::alg;
using namespace geoc::edit;
using namespace geoc::idx;


namespace geoc {
namespace alg {


/** Class to snap vertices of one layer to the close vertices of the other layer. */

class VertexSnapper
{

public:

    /** Default constructor.
      */
    VertexSnapper();

    /** Destructor.
      */
    ~VertexSnapper();

    /** Set reference layer geometries.
      * @param ref Vector of geometries in reference layer.
      */
    void setRefGeometry(TGeomLayer & ref){ refGeometry = ref; }

    /** Set subject layer geometries.
      * @param sub Vector of geometries in subject layer.
      */
    void setSubGeometry(TGeomLayer & sub){ subGeometry = sub; }

    /** Set distance tolerance for snapping.
      * @param tol Maximal distance for snapping.
      */
    void setTolDistance(double tol){ tolDistance = tol; }

    /** Set if geometry correction is wanted */
    void setRepair( bool repair ){ correct = repair; }

    /** Get new geometry of subject layer.
      * @return Changed geometry of subject layer.
      */
    TGeomLayer & getNewGeometry(){ return newGeometry; }

    /** Snap close vertices of subject layer to these of reference layer.
     */
    void snap();

    /** Get list of invalid geometries.
      */
    vector<int> getInvalidGeometries() { return invalids; }

private:

    TGeomLayer refGeometry;
    TGeomLayer subGeometry;
    TGeomLayer newGeometry;
    double tolDistance;
    SpatialIndex* sIndex;
    vector<int> invalids;
    bool correct;

    /** Snap vertices of given geometry to the close vertices from given coordinate sequence.
      * @param geom Pointer to the tested geometry.
      * @param closeCoord Coordinate sequence with close point from the reference layer.
      */
    void snapVertices( GEOCGeom * geom, CoordinateSequence * closeCoord );

};

} //namespace geoc
} //namespace alg

#endif // VERTEXSNAPPER_H
