/***************************************************************************
    aligngeometryeditoroperation.h

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

#ifndef ALIGNGEOMETRYEDITOROPERATION_H
#define ALIGNGEOMETRYEDITOROPERATION_H

// GEOS includes
#include <geos/geom/util/CoordinateOperation.h>
#include <geos/index/strtree/STRtree.h>

// local includes
#include "geoc.h"

using namespace geos::geom::util;

using namespace geoc;
using namespace geoc::geo;
using namespace geoc::alg;
using namespace geoc::edit;
using namespace geoc::tin;
using namespace geoc::idx;

namespace geoc {
namespace edit {

/** Class for editing in coverage alignment by affine transformation. */

class AlignGeometryEditorOperation: public CoordinateOperation
{

    using CoordinateOperation::edit;

public:

    /** Constructor.
      */
    AlignGeometryEditorOperation(): ttin(NULL), sIndex(NULL), changed(false) {}

    /** Destructor
      */
    ~AlignGeometryEditorOperation();

    /** Set TIN.
      */
    void setTIN( TTin *t );

    /** Indicates whether geometry is changed.
      */
    bool isChanged() { return changed; }

    /** Virtual function for editing geometry according to mCoord.
      * @param coordinates Not important.
      * @param geom Geometry to be edited.
      */
    CoordinateSequence* edit(const CoordinateSequence *coordinates, const Geometry *g );

private:

    TTin *ttin;
    CoordinateSequence *idPoints1;
    CoordinateSequence *idPoints2;
    SpatialIndex *sIndex;
    bool changed;

    /** Find identic points for given point - vertices of triangles
      */
    bool findIdPoints( Coordinate *point );

};


} //namespace geoc
} //namespace edit

#endif // ALIGNGEOMETRYEDITOROPERATION_H
