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

/** Class for editing geometry by coordinate sequence. */

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

    /** Virtual function for editing geometry according to mCoord.
     @param coordinates Not important.
     @param geom Geometry to be edited.
     */
    CoordinateSequence* edit(const CoordinateSequence *coordinates, const Geometry *g );


private:

    CoordinateSequence *closeCoord;
    double tolDistance;
    bool changed;

};


#endif // VERTEXGEOMETRYEDITOROPERATION_H
