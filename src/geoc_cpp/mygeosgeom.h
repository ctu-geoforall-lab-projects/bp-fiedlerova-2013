#ifndef MYGEOSGEOM_H
#define MYGEOSGEOM_H

// GEOS includes
#include <geos/geom/GeometryFactory.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/Coordinate.h>
#include <geos/geom/CoordinateSequence.h>
#include <geos/io/WKTReader.h>
#include <geos/io/WKTWriter.h>

using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace geos::io;


/** Class for representing GEOS geometry and supporting parameters. */

class MyGEOSGeom
{

public:

    /** Default constructor
     */
    MyGEOSGeom(): geosGeom( NULL ), matched( NULL ), changed( false ), featureId( 0 ) {}

    /** Constructor
     */
    MyGEOSGeom( Geometry *g, int fid = 0 ): geosGeom( g ), matched( NULL ), changed( false ), featureId( fid ) {}

    /** Sets geometry of this to given Geometry.
     */
    void setGEOSGeom( Geometry *g ) { geosGeom = g; }

    /** Sets geometry of this to given WKTGeometry.
     */
    void setGEOSGeomFromWKT( std::string wkt ) {  geosGeom = geos::io::WKTReader().read( wkt ); }

    /** Sets matching geometry to this.
     */
    void setMatchingGeom( Geometry *m ) { matched = m; }

    /** Returns matching geometry
      */
    Geometry* getMatched() { return matched; }

    /** Returns Geometry of this.
     */
    Geometry*  getGEOSGeom() { return geosGeom; }

    /** Returns geometry of this in wkt.
      */
    std::string getWKTGeom() { return geos::io::WKTWriter().write( geosGeom );}

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

    Geometry * geosGeom;
    Geometry * matched;
    bool changed;
    int featureId;

};


typedef std::vector< MyGEOSGeom > TGeomLayer;

#endif // MYGEOSGEOM_H
