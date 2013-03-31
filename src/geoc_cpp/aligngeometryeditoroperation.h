#ifndef ALIGNGEOMETRYEDITOROPERATION_H
#define ALIGNGEOMETRYEDITOROPERATION_H

// GEOS includes
#include <geos/geom/util/CoordinateOperation.h>
#include <geos/index/strtree/STRtree.h>

// local includes
#include "geoc.h"

using namespace geos::geom::util;

class AlignGeometryEditorOperation: public CoordinateOperation
{

    using CoordinateOperation::edit;

public:

    /** Constructor.
      */
    AlignGeometryEditorOperation(): tin(NULL), sIndex(NULL), changed(false) {}

    /** Destructor
      */
    ~AlignGeometryEditorOperation();

    /** Set TIN.
      */
    void setTIN( TTin *t ){ tin = t; buildIndex(); }

    /** Find identic points for given point - vertices of triangles
      */
    bool findIdPoints( Coordinate *point );

    /** Indicates whether geometry is changed.
      */
    bool isChanged() { return changed; }

    /** Build spatial index.
      */
    void buildIndex();

    /** Virtual function for editing geometry according to mCoord.
     @param coordinates Not important.
     @param geom Geometry to be edited.
     */
    CoordinateSequence* edit(const CoordinateSequence *coordinates, const Geometry *g );

private:

    TTin *tin;
    CoordinateSequence *idPoints1;
    CoordinateSequence *idPoints2;
    SpatialIndex *sIndex;
    bool changed;

};

#endif // ALIGNGEOMETRYEDITOROPERATION_H
