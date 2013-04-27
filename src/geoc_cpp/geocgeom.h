/***************************************************************************
    geocgeom.h

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

#ifndef GEOCGEOM_H
#define GEOCGEOM_H

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

namespace geoc {
namespace geo {

/** Class for internal representation of GEOS geometry and supporting parameters. */

class GEOCGeom
{

public:

    /** Default constructor
     */
    GEOCGeom(): geosGeom( NULL ), matched( NULL ), changed( false ), hasMatch(false), featureId( 0 ) {}

    /** Constructor
     */
    GEOCGeom( Geometry *g, int fid = 0 ): geosGeom( g ), matched( NULL ), changed( false ), hasMatch(false), featureId( fid ) {}

    /** Sets geometry of this to given Geometry.
     */
    void setGEOSGeom( Geometry *g ) { geosGeom = g; }

    /** Sets geometry of this to given WKTGeometry.
     */
    void setGEOSGeomFromWKT( std::string wkt ) {  geosGeom = geos::io::WKTReader().read( wkt ); }

    /** Sets matching geometry to this.
     */
    void setMatchingGeom( Geometry *m ) { hasMatch = true; matched = m; }

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

    /** Returns true if geometry has matching geometry
      */
    bool isMatch() { return hasMatch; }


private:

    Geometry * geosGeom;
    Geometry * matched;
    bool changed;
    bool hasMatch;
    int featureId;

};

typedef std::vector< GEOCGeom > TGeomLayer;

} // namespace geoc
} // namespace geo


#endif // GEOCGEOM_H
