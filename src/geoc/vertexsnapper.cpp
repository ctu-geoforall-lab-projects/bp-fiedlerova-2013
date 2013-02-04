// local includes
#include "vertexsnapper.h"

using namespace geos;
using namespace geom;
using namespace util;

VertexSnapper::VertexSnapper()
{
    //initGEOS(); ???

    tolDistance = 0;

}

void VertexSnapper::snap()
{
    TGeomLayer::iterator sub_it = subGeometry.begin();

    for ( ; subGeometry.end() != sub_it ; ++sub_it )
    {

        // find close features from the reference layer
        GEOSCoordSequence  closeCoord;

        TGeomLayer::iterator ref_it = refGeometry.begin();
        for( ; refGeometry.end() != ref_it ; ++ref_it )
        {

            bool close = isClose(*sub_it, *ref_it);

            if (close)
            {
                // add close coordinates
                closeCoord.add( (*ref_it).getCoordinates() );
            }

        }

        // snap vertex
        snapVertices( &(*sub_it) , &closeCoord );

    }
}


void VertexSnapper::isClose(GEOSGeometry g1, GEOSGeometry g2)
{
    // min distance between geometries is less than tolerance
    if ( g1.distance( &g2 ) < tolDistance )
    {
       return true;
    }

    return false;
}


void VertexSnapper::snapVertices(GEOSGeometry *geom, GEOSCoordSequence &closeCoord)
{
    // tested geometry as coordination sequence
    GEOSCoordSequence coord = geom->getCoordinates();

    // find closest point from closeCoord
    for ( unsigned int i = 0; i < coord.getSize(); i++)
    {
        // minimal distance
        double minDist = coord.getAt(i).distance( closeCoord.getAt(0) );
        unsigned int indMin = 0;
        bool isMin = false;

        for ( unsigned int j = 0; j < closeCoord.getSize(); j++ )
        {
            // compute distance between two tested points
            double dist = coord.getAt(i).distance( closeCoord.getAt(j) );

            if( dist <= minDist )
            {
                minDist = dist;
                indMin = j;
                isMin = true;
            }

        }

        coord.getAt( i, closeCoord.getAt(indMin) );
    }

    // edit geometry
    editGeometry( geom, coord);

}


void VertexSnapper::editGeometry( GEOSGeometry *geom, GEOSCoordSequence &coord )
{
    // edit geometry according to coord -> GeometryEditor and GeometryEditorOperation/interface classes ??????
}
