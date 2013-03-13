// local includes
#include "vertexsnapper.h"

VertexSnapper::VertexSnapper()
{

    tolDistance = 0;

} // constructor


void VertexSnapper::snap()
{

    qDebug("VertexSnapper::snap: ENTERING SNAP");

    for ( unsigned int i = 0; i < subGeometry.size(); i++ )
    {

        // find close features from the reference layer
        CoordinateSequence *closeCoord = new CoordinateArraySequence();

        for ( unsigned int j = 0; j < refGeometry.size(); j++)
        {

            // check if features from sub and ref are close
            bool close = isClose( subGeometry[i], refGeometry[j] );

            if (close)
            {
                // add close coordinates
                closeCoord->add( refGeometry[j].getGEOSGeom()->getCoordinates(), false, true );

            }

        }

        // snap vertex if there are close points
        if ( closeCoord->getSize() > 0 )
        {
            MyGEOSGeom newGeom = subGeometry[i];
            snapVertices( &newGeom, closeCoord );
            newGeometry.push_back( newGeom );
            qDebug("VertexSnapper::snap: Adding close points");
        }
        else
        {
            qDebug("VertexSnapper::snap: No close points");
        }

        delete closeCoord;
    }

    for(unsigned int i = 0; i < newGeometry.size(); i++)
    {
        if ( newGeometry[i].getGEOSGeom() == subGeometry[i].getGEOSGeom() )
            qDebug("VertexSnapper::snap: NOTHING CHANGED AT ALL");
    }

} // void VertexSnapper::snap()


bool VertexSnapper::isClose(MyGEOSGeom & g1, MyGEOSGeom & g2)
{
    qDebug("VertexSnapper::isClose: ENTERING ISCLOSE");

    // min distance between geometries is less than tolerance
    if ( g1.getGEOSGeom()->distance( g2.getGEOSGeom() ) < tolDistance )
    {
       return true;
    }

    return false;

} // bool VertexSnapper::isClose(MyGEOSGeom & g1, MyGEOSGeom & g2)


void VertexSnapper::snapVertices(MyGEOSGeom *geom, CoordinateSequence *closeCoord)
{
    qDebug("VertexSnapper::snapVertices: ENTERING SNAP VERTICES");

    // tested geometry as coordination sequence
    CoordinateSequence* coord = geom->getGEOSGeom()->getCoordinates();

    // find closest point from closeCoord
    for ( unsigned int i = 0; i < coord->getSize(); i++)
    {
        // minimal distance
        double minDist = tolDistance;
        unsigned int indMin = 0;
        bool isMin = false;

        for ( unsigned int j = 0; j < closeCoord->getSize(); j++ )
        {
            // compute distance between two tested points
            double dist = coord->getAt(i).distance( closeCoord->getAt(j) );

            if( dist <= minDist )
            {
                minDist = dist;
                indMin = j;
                isMin = true;
            }

        }

        // set new coordinate to the closest point if there is some
        if ( isMin )
        {
            coord->setAt( closeCoord->getAt(indMin), i );
            geom->setChanged( true );
            qDebug("VertexSnapper::snapVertices: Snapping close point");
        }

    }

    if(geom->getGEOSGeom()->getCoordinates() == coord)
    {
        qDebug("VertexSnapper::snapVertices:No changes");
    }

    // edit geometry
    if( geom->isChanged() )
    {
        editGeometry( geom, coord );
    }


} // void VertexSnapper::snapVertices(MyGEOSGeom *geom, CoordinateSequence *closeCoord)


void VertexSnapper::editGeometry( MyGEOSGeom *geom, CoordinateSequence *coord )
{
    // edit geometry according to coord -> GeometryEditor and GeometryEditorOperation/interface classes ??????

    qDebug("VertexSnapper::editGeometry: ENTERING EDIT GEOMETRY");

    /* FIRST WAY
    CoordinateSequence *c = geom->getGEOSGeom()->getCoordinates();
    c = coord;
    geom->getGEOSGeom()->geometryChanged();*/


    //SECOND WAY

    // create new geometry from coordinate sequence and set it to geom
    GeometryFactory *f = NULL; //geom->getGEOSGeom()->getFactory();

    GeometryTypeId type = geom->getGEOSGeom()->getGeometryTypeId();
    switch ( type )
    {
        case GEOS_POINT:
            geom->setGEOSGeom( f->createPoint(coord) );
            break;
        case GEOS_LINESTRING:
            geom->setGEOSGeom( f->createLineString(coord) );
            break;
        case GEOS_LINEARRING:
            geom->setGEOSGeom( f->createLinearRing(coord) );
            break;
        case GEOS_POLYGON:
            geom->setGEOSGeom( f->createPolygon( f->createLinearRing( coord ), NULL) ); // NOTE: Fails if polygon has holes
            break;
        case GEOS_MULTIPOINT:
            qDebug("VertexSnapper::editGeometry: Multi geometry is not supported yet.");
            break;
        case GEOS_MULTILINESTRING:
            qDebug("VertexSnapper::editGeometry: Multi geometry is not supported yet.");
            break;
        case GEOS_MULTIPOLYGON:
            qDebug("VertexSnapper::editGeometry: Multi geometry is not supported yet.");
            break;
        case GEOS_GEOMETRYCOLLECTION:
            qDebug("VertexSnapper::editGeometry: Multi geometry is not supported yet.");
            break;
        default:
            qDebug("VertexSnapper::editGeometry: Unknown geometry type.");
    }


   // THIRD WAY
    /*MyGeometryEditorOperation myOp( coord );

    GeometryEditor geomEdit( geom->getGEOSGeom()->getFactory() );

    // set geometry to edited one
    geom->setGEOSGeom( geomEdit.edit( geom->getGEOSGeom() , &myOp ) );*/


    /* FOURTH WAY
    MyCoordinateSequenceFilter *filter = new MyCoordinateSequenceFilter( coord );
    geom.getGEOSGeom()->apply_rw( *filter );

    geom.getGEOSGeom()->geometryChanged();*/

    if( geom->getGEOSGeom()->isValid() )
    {
        qDebug("VertexSnapper::editGeometry: Geom is valid.");
    }


} // void VertexSnapper::editGeometry( MyGEOSGeom *geom, CoordinateSequence *coord )
