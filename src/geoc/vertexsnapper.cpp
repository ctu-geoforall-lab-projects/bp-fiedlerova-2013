// local includes
#include "vertexsnapper.h"
#include "mygeometryeditoroperation.h"

VertexSnapper::VertexSnapper()
{

    tolDistance = 0;

}

void VertexSnapper::snap()
{
    TGeomLayer::iterator sub_it = subGeometry.begin();

    for ( ; subGeometry.end() != sub_it ; ++sub_it )
    {

        // find close features from the reference layer
        CoordinateSequence *closeCoord = new CoordinateArraySequence();

        TGeomLayer::iterator ref_it = refGeometry.begin();
        for( ; refGeometry.end() != ref_it ; ++ref_it )
        {

            bool close = isClose(*sub_it, *ref_it);

            if (close)
            {
                // add close coordinates
                closeCoord->add( (*ref_it).getGEOSGeom()->getCoordinates(), false, true );

            }

        }

        // snap vertex
        snapVertices( &(*sub_it) , *closeCoord );

    }
}


bool VertexSnapper::isClose(MyGEOSGeom g1, MyGEOSGeom g2)
{
    // min distance between geometries is less than tolerance
    if ( g1.getGEOSGeom()->distance( g2.getGEOSGeom() ) < tolDistance )
    {
       return true;
    }

    return false;
}


void VertexSnapper::snapVertices(MyGEOSGeom *geom, CoordinateSequence &closeCoord)
{
    // tested geometry as coordination sequence
    CoordinateSequence* coord = new CoordinateArraySequence();
    coord = geom->getGEOSGeom()->getCoordinates();

    // find closest point from closeCoord
    for ( unsigned int i = 0; i < coord->getSize(); i++)
    {
        // minimal distance
        double minDist = coord->getAt(i).distance( closeCoord.getAt(0) );
        unsigned int indMin = 0;
        bool isMin = false;

        for ( unsigned int j = 0; j < closeCoord.getSize(); j++ )
        {
            // compute distance between two tested points
            double dist = coord->getAt(i).distance( closeCoord.getAt(j) );

            if( dist <= minDist && dist <= tolDistance)
            {
                minDist = dist;
                indMin = j;
                isMin = true;
            }

        }

        // set new coordinate to the closest point if there is some
        if ( isMin )
        {
            coord->setAt( closeCoord.getAt(indMin), i );
        }

    }

    // edit geometry
    editGeometry( geom, *coord);

}


void VertexSnapper::editGeometry( MyGEOSGeom *geom, CoordinateSequence &coord )
{
    // edit geometry according to coord -> GeometryEditor and GeometryEditorOperation/interface classes ??????

    MyGeometryEditorOperation myOp;
    myOp.setCoordSeq(&coord);

    GeometryEditor *geomEdit = NULL;
    Geometry *g = geom->getGEOSGeom();

    // return edited geometry
    geom->setGEOSgeom( geomEdit->edit( g , &myOp ));

}
