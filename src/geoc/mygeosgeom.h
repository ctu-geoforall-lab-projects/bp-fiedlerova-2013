#ifndef MYGEOSGEOM_H
#define MYGEOSGEOM_H

// GEOS includes
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateSequence.h>


using namespace std;
using namespace geos;
using namespace geos::geom;


/** Class for representing GEOS geometry and supporting parameters. */

class MyGEOSGeom
{

public:

    /** Constructor
     */
    MyGEOSGeom(): geos( NULL ), changed( false ), featureId( 0 ) {}

    /** Sets geometry of this to given Geometry.
     */
    void setGEOSgeom(Geometry *g) { geos = g; }

    /** Returns Geometry of this.
     */
    Geometry*  getGEOSGeom() { return geos; }

    /** Sets id of feature with this geometry.
     */
    void setFeatureId(int id) { featureId = id; }

    /** Returns id of feature with this geometry.
     */
    int getFeatureId() { return featureId; }

    /** Sets if the original geometry was changed.
     */
    void setChanged(bool ch) { changed = ch; }

    /** Returns true if the original geometry was changed.
     */
    bool isChanged() { return changed; }


private:

    Geometry * geos;
    bool changed;
    int featureId;

};

#endif // MYGEOSGEOM_H
