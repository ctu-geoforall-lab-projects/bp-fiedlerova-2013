// local includes
#include "vertexsnapper.h"

VertexSnapper::VertexSnapper()
{

    qDebug("ENTERING CONSTRUCTOR");

    tolDistance = 100;
    refGeometry.clear();
    subGeometry.clear();
    newGeometry.clear();

} // constructor

void VertexSnapper::snap()
{

    qDebug("ENTERING SNAP");

    //TGeomLayer::iterator sub_it = subGeometry.begin();

    for(unsigned int i = 0; i < subGeometry.size(); i++)
    {
        if(subGeometry[i].getGEOSGeom() == refGeometry[i].getGEOSGeom())
            qDebug("SUB IS REF!!!!!");
    }

    for ( unsigned int i = 0; i < subGeometry.size(); i++ ) //; subGeometry.end() != sub_it ; ++sub_it )
    {

        // find close features from the reference layer

        // vectors of coordinates of close Geometries
        std::vector<double> closeCoordX;
        std::vector<double> closeCoordY;

        qDebug("Vectors with coordinates created");

        //TGeomLayer::iterator ref_it = refGeometry.begin();
        for ( unsigned int j = 0; j < refGeometry.size(); j++) //; refGeometry.end() != ref_it ; ++ref_it )
        {

            bool close = isClose( subGeometry[i], refGeometry[j] );//isClose(*sub_it, *ref_it);

            if (close)
            {

                // add close coordinates
                //closeCoord->add( refGeometry[j].getGEOSGeom()->getCoordinates(), false, true );//(*ref_it).getGEOSGeom()->getCoordinates(), false, true );

                // get points from geometry
                GEOSCoordSequence *coords = GEOSCoordSeq_clone( GEOSGeom_getCoordSeq( refGeometry[j].getGEOSGeom() ) );
                qDebug("GEOSCoordSequence cloned from refGeometry");

                // get number of points
                unsigned int cSize;
                GEOSCoordSeq_getSize( coords, &cSize );

                // add x and y coordinates to the vectors
                for ( unsigned int i = 0; i < cSize; i++ )
                {
                    double x, y;
                    GEOSCoordSeq_getX( coords, i, &x );
                    GEOSCoordSeq_getY( coords, i, &y );

                    closeCoordX.push_back(x);
                    closeCoordY.push_back(y);
                    qDebug("Close coordinates x, y added to vector");
                }

            }

        }

        // create sequence with close points
        GEOSCoordSequence *closeCoord = GEOSCoordSeq_create( closeCoordX.size(), 3 );
        qDebug("GEOSCoordSequence closeCoord created");

        for( unsigned int k = 0; k < closeCoordX.size(); k++)
        {
            GEOSCoordSeq_setX(closeCoord, k, closeCoordX[k]);
            GEOSCoordSeq_setY(closeCoord, k, closeCoordX[k]);
        }

        // snap vertex
        newGeometry.push_back( snapVertices( subGeometry[i], *closeCoord) );//&(*sub_it) , *closeCoord );

    }

    for(unsigned int i = 0; i < newGeometry.size(); i++)
    {
        if(newGeometry[i].getGEOSGeom() == subGeometry[i].getGEOSGeom())
            qDebug("NOTHING CHANGED AT ALL");
    }

} // void VertexSnapper::snap()


bool VertexSnapper::isClose(MyGEOSGeom & g1, MyGEOSGeom & g2)
{
    qDebug("ENTERING ISCLOSE");

    // distance
    double dist;
    GEOSDistance(g1.getGEOSGeom(), g2.getGEOSGeom(), &dist);
    qDebug("distance computed");

    // min distance between geometries is less than tolerance
    if ( dist < tolDistance )
    {
       //delete dist;
       return true;
    }

    //delete dist;
    return false;


} // bool VertexSnapper::isClose(MyGEOSGeom & g1, MyGEOSGeom & g2)


MyGEOSGeom& VertexSnapper::snapVertices(MyGEOSGeom &geom, GEOSCoordSequence &closeCoord)
{
    qDebug("ENTERING SNAP VERTICES");

    // tested geometry as coordination sequence
    GEOSCoordSequence *coord =  GEOSCoordSeq_clone( GEOSGeom_getCoordSeq( geom.getGEOSGeom() ) );

    //first point from closePoints
    double x0, y0;
    GEOSCoordSeq_getX( &closeCoord, 0, &x0);
    GEOSCoordSeq_getY( &closeCoord, 0, &y0);
    GEOSCoordSequence *point0 = GEOSCoordSeq_create( 1, 3 );
    GEOSCoordSeq_setX( point0, 0, x0);
    GEOSCoordSeq_setY( point0, 0, y0);
    GEOSGeometry * pointGeom0 = GEOSGeom_createPoint( point0 );

    // get number of points
    unsigned int cSize;
    GEOSCoordSeq_getSize( coord, &cSize );

    unsigned int ccSize;
    GEOSCoordSeq_getSize( &closeCoord, &ccSize );

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
            GEOSCoordSeq_getX( &closeCoord, indMin, &newX);
            GEOSCoordSeq_getY( &closeCoord, indMin, &newY);
            GEOSCoordSeq_setX( coord, i, newX);
            GEOSCoordSeq_setY( coord, i, newY);
            //coord->setAt( closeCoord.getAt(indMin), i );
        }

    }

    // edit geometry
    editGeometry( geom, *coord);

    return geom;

} // MyGEOSGeom& VertexSnapper::snapVertices(MyGEOSGeom &geom, CoordinateSequence &closeCoord)


void VertexSnapper::editGeometry( MyGEOSGeom &geom, GEOSCoordSequence &coord )
{
    // edit geometry according to coord -> GeometryEditor and GeometryEditorOperation/interface classes ??????

    qDebug("ENTERING EDIT GEOMETRY");    

    //MyGeometryEditorOperation myOp( &coord );

    //GeometryEditor geomEdit;
    //GEOSGeometry* g = geom.getGEOSGeom();

    // return edited geometry
    //geom.setGEOSGeom( geomEdit.edit( g , &myOp ) );


    //if( geom.getGEOSGeom()->isValid() )
      //  qDebug("Geom is valid.");


} // void VertexSnapper::editGeometry( MyGEOSGeom &geom, CoordinateSequence &coord )
