// local includes
#include "vertexsnapper.h"

#include<iostream>

VertexSnapper::VertexSnapper()
{

    tolDistance = 0;

} // constructor


void VertexSnapper::snap()
{
    newGeometry = subGeometry;

    //TGeomLayer::iterator newIt;
    //TGeomLayer::iterator refIt;

    for ( unsigned int i = 0; i < subGeometry.size(); i++ )
    //for ( newIt = newGeometry.begin(); newIt != newGeometry.end(); newIt++ )
    {
        // find close features from the reference layer
        CoordinateSequence *closeCoord = new CoordinateArraySequence();

        for ( unsigned int j = 0; j < refGeometry.size(); j++)
        //for ( refIt = refGeometry.begin(); refIt != refGeometry.end(); refIt++ )
        {
            // check if features from sub and ref are close
            bool close = isClose( subGeometry[i], refGeometry[j]); // *newIt, *refIt );
            if (close)
            {
                // add close coordinates
                //closeCoord->add( (*refIt).getGEOSGeom()->getCoordinates(), true, true );
                closeCoord->add(refGeometry[j].getGEOSGeom()->getCoordinates(), true, true );
            }

        }

        // snap vertex if there are close points
        if ( closeCoord->getSize() > 0 )
        {
            MyGEOSGeom newGeom = subGeometry[i]; //*newIt;
            snapVertices( &newGeom, closeCoord );
            newGeometry[i] = newGeom; // *newIt = newGeom;
        }

        delete closeCoord;

    }

} // void VertexSnapper::snap()


bool VertexSnapper::isClose(MyGEOSGeom & g1, MyGEOSGeom & g2)
{
    // min distance between geometries is less than tolerance
    if ( g1.getGEOSGeom()->distance( g2.getGEOSGeom() ) < tolDistance )
    {
       return true;
    }

    return false;

} // bool VertexSnapper::isClose(MyGEOSGeom & g1, MyGEOSGeom & g2)


void VertexSnapper::snapVertices(MyGEOSGeom *geom, CoordinateSequence *closeCoord)
{
    // tested geometry as coordination sequence
    //CoordinateSequence* coord = geom->getGEOSGeom()->getCoordinates();

    VertexGeometryEditorOperation myOp;
    myOp.setCloseCoordSeq( closeCoord );
    myOp.setTolDistance( tolDistance );

    qDebug("Initializing");

    GeometryEditor geomEdit( geom->getGEOSGeom()->getFactory() );

    // set geometry to edited one
    geom->setGEOSGeom( geomEdit.edit( geom->getGEOSGeom() , &myOp ) );

    qDebug("Editing");

    // check if geometry was changed
    geom->setChanged( myOp.isChanged() );

    qDebug("Validing");

    // check validity
    if( !geom->getGEOSGeom()->isValid() )
    {
        qDebug("VertexSnapper::editGeometry: Geom is not valid.");
    }


} // void VertexSnapper::snapVertices(MyGEOSGeom *geom, CoordinateSequence *closeCoord)


void VertexSnapper::editGeometry( MyGEOSGeom *geom, CoordinateSequence *coord )
{
    // edit geometry according to coord -> GeometryEditor and GeometryEditorOperation/interface classes ??????

    // FIRST WAY
    /*CoordinateSequence *c = geom->getGEOSGeom()->getCoordinates();
    c = coord;
    geom->getGEOSGeom()->geometryChanged();*/


    //SECOND WAY

    // create new geometry from coordinate sequence and set it to geom
    /*GeometryFactory *f = NULL; //geom->getGEOSGeom()->getFactory();

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
    }*/


   // THIRD WAY
    /*if( !geom->getGEOSGeom()->isValid() )
    {
        qDebug("VertexSnapper::editGeometry: Geom is not valid before editing.");
    }

    VertexGeometryEditorOperation myOp;

    GeometryEditor geomEdit( geom->getGEOSGeom()->getFactory() );

    // set geometry to edited one
    geom->setGEOSGeom( geomEdit.edit( geom->getGEOSGeom() , &myOp ) );*/


    /* FOURTH WAY
    MyCoordinateSequenceFilter *filter = new MyCoordinateSequenceFilter( coord );
    geom.getGEOSGeom()->apply_rw( *filter );

    geom.getGEOSGeom()->geometryChanged();*/


} // void VertexSnapper::editGeometry( MyGEOSGeom *geom, CoordinateSequence *coord )
