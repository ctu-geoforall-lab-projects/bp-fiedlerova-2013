#ifndef MYGEOSGEOM_H
#define MYGEOSGEOM_H

// GEOS includes
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/io/WKTReader.h>
#include <geos/io/WKTWriter.h>

// local includes
#include "geoc.h"

using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace geos::io;


/** Class for representing GEOS geometry and supporting parameters. */

class MyGEOSGeom
{

public:

    /** Constructor
     */
    MyGEOSGeom(): geos( NULL ), changed( false ), featureId( 0 ) {}

    /** Sets geometry of this to given Geometry.
     */
    void setGEOSGeom( Geometry *g ) { geos = g; }

    /** Sets geometry of this to given WKTGeometry.
     */
    void setGEOSGeomFromWKT( std::string wkt ) {  geos = geos::io::WKTReader().read( wkt ); }

    /** Returns Geometry of this.
     */
    Geometry*  getGEOSGeom() { return geos; }

    /** Returns geometry of this in wkt.
      */
    std::string getWKTGeom() { return geos::io::WKTWriter().write( geos );}

    /** Sets id of feature with this geometry.
     */
    void setFeatureId( int id ) { featureId = id; }

    /** Returns id of feature with this geometry.
     */
    int getFeatureId() { return featureId; }

    /** Sets if the original geometry was changed.
     */
    void setChanged( bool ch ) { changed = ch; }

    /** Returns true if the original geometry was changed.
     */
    bool isChanged() { return changed; }


private:

    Geometry * geos;
    bool changed;
    int featureId;

};

typedef std::vector< MyGEOSGeom > TGeomLayer;

#endif // MYGEOSGEOM_H
