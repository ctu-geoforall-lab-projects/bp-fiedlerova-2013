#ifndef GEOMETRYCORRECTIONOPERATION_H
#define GEOMETRYCORRECTIONOPERATION_H

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

/** Class for editing geometry - repair invalid geometry. */

class GeometryCorrectionOperation: public CoordinateOperation
{

    using CoordinateOperation::edit;

public:

    /// Constructor
    GeometryCorrectionOperation() {}

    /** Virtual function for editing - repairing geometry.
     * @param coordinates Not important.
     * @param g Geometry to be edited.
     */
    CoordinateSequence* edit(const CoordinateSequence *coordinates, const Geometry *g );

    /** Remove dead branches
      * @param line Line with possible dead branches.
      */
    void removeDeadBranch( CoordinateSequence *line );

    /** Remove crosses by reverse orientation of segment
      * @param line Line with possible crosses.
      */
    void removeCrosses( CoordinateSequence *line );

};

} //namespace geoc
} //namespace edit


#endif // GEOMETRYCORRECTIONOPERATION_H
