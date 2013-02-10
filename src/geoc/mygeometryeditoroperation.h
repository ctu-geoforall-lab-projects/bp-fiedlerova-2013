#ifndef MYGEOMETRYEDITOROPERATION_H
#define MYGEOMETRYEDITOROPERATION_H

// local includes
#include "mygeosgeom.h"

// GEOS includes
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/util/CoordinateOperation.h>

using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace geos::geom::util;

/** Class for editing geometry by coordinate sequence. */

class MyGeometryEditorOperation: public CoordinateOperation
{

    using CoordinateOperation::edit;

private:

    CoordinateSequence *mCoord;

public:

    /// Constructor
    MyGeometryEditorOperation(){}

    /** Set private mCoord to given coord.
     */
    void setCoordSeq( CoordinateSequence *coord ) { mCoord = coord; }

    /** Virtual function for editing geometry according to mCoord.
     @param coordinates Not important.
     @param geom Geometry to be edited.
     */
    CoordinateSequence* edit(const CoordinateSequence *coordinates, const Geometry *geom)
    { coordinates = mCoord; return mCoord; }

};

#endif // MYGEOMETRYEDITOROPERATION_H
