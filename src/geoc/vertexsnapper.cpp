// local includes
#include "vertexsnapper.h"

VertexSnapper::VertexSnapper()
{

    qDebug("VertexSnapper::VertexSnapper: ENTERING CONSTRUCTOR");

    tolDistance = 1;
    refGeometry.clear();
    subGeometry.clear();
    newGeometry.clear();

} // constructor

void VertexSnapper::snap()
{

    qDebug("VertexSnapper::snap: ENTERING SNAP");

    for ( unsigned int i = 0; i < subGeometry.size(); i++ )
    {

        // find close features from the reference layer

        // vectors of coordinates of close Geometries
        std::vector<double> closeCoordX;
        std::vector<double> closeCoordY;

        qDebug("VertexSnapper::snap: Vectors with coordinates created");

        //TGeomLayer::iterator ref_it = refGeometry.begin();
        for ( unsigned int j = 0; j < refGeometry.size(); j++)
        {
            // some needed parameters from refGeometry
            /*unsigned int size, dim;
            GEOSCoordSeq_getSize( GEOSGeom_getCoordSeq( refGeometry[j].getGEOSGeom() ), &size );
            GEOSCoordSeq_getDimensions( GEOSGeom_getCoordSeq( refGeometry[j].getGEOSGeom() ), &dim );*/

            bool close = isClose( subGeometry[i].getGEOSGeom(), refGeometry[j].getGEOSGeom() );
            qDebug("VertexSnapper::snap: isClose checked.");

            if (close)
            {

                // add close coordinates
                //closeCoord->add( refGeometry[j].getGEOSGeom()->getCoordinates(), false, true );//(*ref_it).getGEOSGeom()->getCoordinates(), false, true );

                // get points from geometry
                const GEOSCoordSequence *s = GEOSGeom_getCoordSeq( refGeometry[j].getGEOSGeom() ); // NOTE: Only linestring or point is possible
                qDebug("VertexSnapper::snap: CoordSeq created.");
                GEOSCoordSequence *coords = GEOSCoordSeq_clone( s );
                /*GEOSCoordSequence *coords = GEOSCoordSeq_create( size, dim );

                for ( unsigned int m = 0; m < size; m++ )
                {
                    double x, y;
                    GEOSCoordSeq_getX( s, m, &x);
                    GEOSCoordSeq_getY( s, m, &y);

                    GEOSCoordSeq_setX( coords, m, x );
                    GEOSCoordSeq_setY( coords, m, y );
                }*/

                qDebug("VertexSnapper::snap: GEOSCoordSequence cloned from refGeometry");

                // get number of points
                unsigned int cSize;
                GEOSCoordSeq_getSize( coords, &cSize );

                // add x and y coordinates to the vectors
                for ( unsigned int k = 0; k < cSize; k++ )
                {
                    double x, y;
                    GEOSCoordSeq_getX( coords, k, &x );
                    GEOSCoordSeq_getY( coords, k, &y );

                    closeCoordX.push_back(x);
                    closeCoordY.push_back(y);
                    qDebug("VertexSnapper::snap: Close coordinates x, y added to vector");
                }

            }

        }

        // create sequence with close points
        GEOSCoordSequence *closeCoord = GEOSCoordSeq_create( closeCoordX.size(), 2 );
        qDebug("VertexSnapper::snap: GEOSCoordSequence closeCoord created");

        for( unsigned int l = 0; l < closeCoordX.size(); l++)
        {
            GEOSCoordSeq_setX(closeCoord, l, closeCoordX[l]);
            GEOSCoordSeq_setY(closeCoord, l, closeCoordX[l]);
        }

        // snap vertex
        snapVertices( &subGeometry[i], closeCoord);
        newGeometry.push_back( subGeometry[i] );

    }

    for(unsigned int i = 0; i < newGeometry.size(); i++)
    {
        if(newGeometry[i].getGEOSGeom() == subGeometry[i].getGEOSGeom())
            qDebug("VertexSnapper::snap: Nothing changed");
    }

} // void VertexSnapper::snap()


bool VertexSnapper::isClose( const GEOSGeometry * g1, const GEOSGeometry * g2)
{
    qDebug("VertexSnapper::isClose: ENTERING ISCLOSE");

    // distance
    double dist = tolDistance;
    GEOSDistance( g1, g2, &dist );
    qDebug("VertexSnapper::isClose: Distance computed");

    // min distance between geometries is less than tolerance
    if ( dist < tolDistance )
    {
       return true;
    }

    return false;

} // bool VertexSnapper::isClose(MyGEOSGeom & g1, MyGEOSGeom & g2)


void VertexSnapper::snapVertices(MyGEOSGeom *geom, GEOSCoordSequence *closeCoord)
{
    qDebug("VertexSnapper::snapVertices: ENTERING SNAP VERTICES");

    // tested geometry as coordination sequence
    const GEOSCoordSequence *s = GEOSGeom_getCoordSeq( geom->getGEOSGeom() );
    GEOSCoordSequence *coord =  GEOSCoordSeq_clone( s );

    /*unsigned int size, dim;
    GEOSCoordSeq_getSize( s, &size );
    GEOSCoordSeq_getDimensions( s, &dim );
    GEOSCoordSequence *coord = GEOSCoordSeq_create( size, dim );

    for ( unsigned int m = 0; m < size; m++ )
    {
        double x, y;
        GEOSCoordSeq_getX( s, m, &x);
        GEOSCoordSeq_getY( s, m, &y);

        GEOSCoordSeq_setX( coord, m, x );
        GEOSCoordSeq_setY( coord, m, y );
    }*/

    qDebug("VertexSnapper::snapVertices: GEOSCoordSequence cloned from geom Geometry");


    //first point from closePoints
    double x0, y0;
    GEOSCoordSeq_getX( closeCoord, 0, &x0);
    GEOSCoordSeq_getY( closeCoord, 0, &y0);
    GEOSCoordSequence *point0 = GEOSCoordSeq_create( 1, 3 );
    GEOSCoordSeq_setX( point0, 0, x0);
    GEOSCoordSeq_setY( point0, 0, y0);
    GEOSGeometry * pointGeom0 = GEOSGeom_createPoint( point0 );

    // get number of points
    unsigned int cSize;
    GEOSCoordSeq_getSize( coord, &cSize );

    unsigned int ccSize;
    GEOSCoordSeq_getSize( closeCoord, &ccSize );

    // find closest point from closeCoord
    for ( unsigned int i = 0; i < cSize; i++)
    {
        // get point from coordinate sequence
        double x, y;
        GEOSCoordSeq_getX( coord, i, &x);
        GEOSCoordSeq_getY( coord, i, &y);
        GEOSCoordSequence *point = GEOSCoordSeq_create( 1, 3 );
        GEOSCoordSeq_setX( point, 0, x);
        GEOSCoordSeq_setY( point, 0, y);
        GEOSGeometry * pointGeom = GEOSGeom_createPoint( point );

        // minimal distance
        double minDist;// = coord->getAt(i).distance( closeCoord.getAt(0) );
        GEOSDistance( pointGeom0, pointGeom, &minDist );

        unsigned int indMin = 0;
        bool isMin = false;

        for ( unsigned int j = 0; j < ccSize; j++ )
        {
            // get point from coordinate sequence
            double xx, yy;
            GEOSCoordSeq_getX( coord, j, &xx);
            GEOSCoordSeq_getY( coord, j, &yy);
            GEOSCoordSequence *pointj = GEOSCoordSeq_create( 1, 3 );
            GEOSCoordSeq_setX( pointj, 0, xx);
            GEOSCoordSeq_setY( pointj, 0, yy);
            GEOSGeometry * pointGeomj = GEOSGeom_createPoint( pointj );

            // compute distance between two tested points
            double dist =  GEOSDistance( pointGeomj, pointGeom, &minDist ); //coord->getAt(i).distance( closeCoord.getAt(j) );

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
            double newX, newY;
            GEOSCoordSeq_getX( closeCoord, indMin, &newX);
            GEOSCoordSeq_getY( closeCoord, indMin, &newY);
            GEOSCoordSeq_setX( coord, i, newX);
            GEOSCoordSeq_setY( coord, i, newY);
            //coord->setAt( closeCoord.getAt(indMin), i );
        }

    }

    // edit geometry
    editGeometry( geom, coord);

} // MyGEOSGeom& VertexSnapper::snapVertices(MyGEOSGeom &geom, CoordinateSequence &closeCoord)


void VertexSnapper::editGeometry( MyGEOSGeom *geom, GEOSCoordSequence *coord )
{
    // edit geometry according to coord -> GeometryEditor and GeometryEditorOperation/interface classes ??????

    qDebug("VertexSnapper::editGeometry: ENTERING EDIT GEOMETRY");

    // new geometry
    GEOSGeometry *newGeom = NULL;
    GEOSGeometry * ring = NULL;

    // change geometry according to its type  // NOTE: improve this according to http://trac.osgeo.org/geos/browser/trunk/tests/geostest/geostest.c - fineGrainedReconstructionTest
    int type = GEOSGeomTypeId( geom->getGEOSGeom() );
    switch ( type )
    {
        case GEOS_POINT:
            newGeom = GEOSGeom_createPoint( coord );
            break;
        case GEOS_LINESTRING:
            newGeom = GEOSGeom_createLineString( coord );
            break;
        case GEOS_LINEARRING:
            newGeom = GEOSGeom_createLinearRing( coord );
            break;
        case GEOS_POLYGON:
            ring = GEOSGeom_createLinearRing( coord ); // NOTE: Fails if polygon has holes
            newGeom = GEOSGeom_createPolygon( ring, NULL, 0 );
            break;
        case GEOS_MULTIPOINT:
            newGeom = GEOSGeom_createEmptyCollection(4);
            qDebug("VertexSnapper::editGeometry: Multi geometry is not supported yet.");
            break;
        case GEOS_MULTILINESTRING:
            newGeom = GEOSGeom_createEmptyCollection(5);
            qDebug("VertexSnapper::editGeometry: Multi geometry is not supported yet.");
            break;
        case GEOS_MULTIPOLYGON:
            newGeom = GEOSGeom_createEmptyCollection(6);
            qDebug("VertexSnapper::editGeometry: Multi geometry is not supported yet.");
            break;
        case GEOS_GEOMETRYCOLLECTION:
            newGeom = GEOSGeom_createEmptyCollection(7);
            qDebug("VertexSnapper::editGeometry: Multi geometry is not supported yet.");
            break;
        default:
            qDebug("VertexSnapper::editGeometry: Unknown geometry type.");
    }


    // return edited geometry
    geom->setGEOSGeom( newGeom );


    if( GEOSisEmpty( geom->getGEOSGeom() ) )
        qDebug("VertexSnapper::editGeometry: Geom is empty.");

    if( GEOSisValid( geom->getGEOSGeom() ) )
        qDebug("VertexSnapper::editGeometry: Geom is valid.");


} // void VertexSnapper::editGeometry( MyGEOSGeom &geom, CoordinateSequence &coord )
