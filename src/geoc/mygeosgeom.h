#ifndef MYGEOSGEOM_H
#define MYGEOSGEOM_H

// GEOS includes
#include <geos_c.h>

using namespace std;


/** Class for representing GEOS geometry and supporting parameters. */

class MyGEOSGeom
{

public:

    /** Default constructor
     */
    MyGEOSGeom(): geosGeom( NULL ), matched( NULL ), changed( false ), featureId( 0 ) {}

    /** Constructor
     */
    MyGEOSGeom( GEOSGeometry *g, int fid = 0 ): geosGeom( g ), matched( NULL ), changed( false ), featureId( fid ) {}

    /** Destructor
      */
    //~MyGEOSGeom() { if( geosGeom ){ GEOSGeom_destroy( geosGeom ); }  }

    /** Sets geometry of this to given Geometry.
     */
    void setGEOSGeom( GEOSGeometry *g ) { geosGeom = g; }

    /** Sets matching geometry to this.
     */
    void setMatchingGeom( MyGEOSGeom *m ) { matched = m; }

    /** Returns Geometry of this.
     */
    const GEOSGeometry*  getGEOSGeom() const { return geosGeom; }

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

    GEOSGeometry * geosGeom;
    MyGEOSGeom * matched;
    bool changed;
    int featureId;

};


typedef vector< MyGEOSGeom > TGeomLayer;

#endif // MYGEOSGEOM_H
