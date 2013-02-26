// local includes
#include "vertexsnapper.h"

VertexSnapper::VertexSnapper()
{

    qDebug("ENTERING CONSTRUCTOR");

    tolDistance = 100;
    refGeometry.clear();
    subGeometry.clear();
    newGeometry.clear();

}

void VertexSnapper::snap()
{

    qDebug("ENTERING SNAP");

    //TGeomLayer::iterator sub_it = subGeometry.begin();

    for ( unsigned int i = 0; i < subGeometry.size(); i++ ) //; subGeometry.end() != sub_it ; ++sub_it )
    {

        // find close features from the reference layer
        CoordinateSequence *closeCoord = new CoordinateArraySequence();

        //TGeomLayer::iterator ref_it = refGeometry.begin();
        for ( unsigned int j = 0; j < refGeometry.size(); j++) //; refGeometry.end() != ref_it ; ++ref_it )
        {

            bool close = isClose( subGeometry[i], refGeometry[j] );//isClose(*sub_it, *ref_it);

            if (close)
            {
                // add close coordinates
                closeCoord->add( refGeometry[j].getGEOSGeom()->getCoordinates(), false, true );//(*ref_it).getGEOSGeom()->getCoordinates(), false, true );

            }

        }

        // snap vertex
        newGeometry.push_back( snapVertices( subGeometry[i], *closeCoord) );//&(*sub_it) , *closeCoord );

    }

}


bool VertexSnapper::isClose(MyGEOSGeom & g1, MyGEOSGeom & g2)
{
    qDebug("ENTERING ISCLOSE");

    // min distance between geometries is less than tolerance
    if ( g1.getGEOSGeom()->distance( g2.getGEOSGeom() ) < tolDistance )
    {
       return true;
    }

    return false;
}


MyGEOSGeom& VertexSnapper::snapVertices(MyGEOSGeom &geom, CoordinateSequence &closeCoord)
{
    qDebug("ENTERING SNAP VERTICES");

    // tested geometry as coordination sequence
    CoordinateSequence* coord = new CoordinateArraySequence();
    coord = geom.getGEOSGeom()->getCoordinates();

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

    return geom;
}


void VertexSnapper::editGeometry( MyGEOSGeom &geom, CoordinateSequence &coord )
{
    // edit geometry according to coord -> GeometryEditor and GeometryEditorOperation/interface classes ??????

    qDebug("ENTERING EDIT GEOMETRY");

    MyGeometryEditorOperation myOp( &coord );

    qDebug("Coordinates set.");

    GeometryEditor geomEdit;
    Geometry *g = geom.getGEOSGeom();
    //geomEdit.reset( new GeometryEditor() );

    if(geom.getGEOSGeom()->isValid())
        qDebug("Geom set.");

    // return edited geometry
    geom.setGEOSGeom( geomEdit.edit( g , &myOp ) );

    if(geom.getGEOSGeom()->isValid() && !geom.getGEOSGeom()->equals(g))
        qDebug("Geom is valid.");

}
