#ifndef MYCOORDINATESEQUENCEFILTER_H
#define MYCOORDINATESEQUENCEFILTER_H

// GEOS includes
#include <geos/geom/Geometry.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/CoordinateSequenceFilter.h>

// local includes
#include "mygeosgeom.h"
#include "geoc.h"

using namespace geos;
using namespace geos::geom;

class MyCoordinateSequenceFilter: public CoordinateSequenceFilter
{

public:

    MyCoordinateSequenceFilter( CoordinateSequence & s ): seq(s), done(false) {}

    void filter_ro(const CoordinateSequence &, size_t);
    void filter_rw(CoordinateSequence &, size_t);
    bool isDone() const;
    bool isGeometryChanged() const;

private:
    CoordinateSequence& seq;
    bool done;

};

#endif // MYCOORDINATESEQUENCEFILTER_H
