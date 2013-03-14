#include "matchinggeometry.h"

MatchingGeometry::MatchingGeometry()
{
    tolDistance = 1;
    geometrySet = NULL;

}

void MatchingGeometry::setGeometrySet( TGeomLayer * geomSet)
{
    qDebug("MatchingGeometry::setGeometrySet: ENTERING ");

    // set the right geometrySet
    geometrySet = geomSet;

} // void MatchingGeometry::setGeometrySet( TGeomLayer geometrySet, bool isFirst )


void  MatchingGeometry::closeGeometries( const Geometry *geom)
{
    qDebug("MatchingGeometry::closeGeometries: ENTERING ");

    // find close geometries
    unsigned long size = geometrySet->size();
    closeSet.clear();

    for ( unsigned long i = 0; i < size; i++ )
    {
        // test wether geometry from set is within given tolerance distance from tested geometry
        if ( isClose( geometrySet->at(i).getGEOSGeom(), geom ) )
        {
            closeSet.push_back( geometrySet->at(i) );

        }

    }

} // void  MatchingGeometry::closeGeometries( TGeomLayer * closeSet, const Geometry *geom, bool isFirst )


bool MatchingGeometry::isClose( const Geometry * g1, const Geometry * g2 )
{
    qDebug("MatchingGeometry::isClose: ENTERING ");

    double dist = g1->distance( g2 );

    // test distance between g1 and g2 and tolerance distance
    if( dist < tolDistance )
    {
        return true;
    }

    return false;

} // bool MatchingGeometry::isClose( const Geometry * g1, const Geometry * g2 )


Geometry* MatchingGeometry::buffer( Geometry *geom )
{
    qDebug("MatchingGeometry::buffer: ENTERING ");

    // buffer
    return geom->buffer( tolDistance );

} // void MatchingGeometry::buffer( Geometry *buffer, const Geometry *geom )


Geometry* MatchingGeometry::boundary( Geometry *geom )
{
    qDebug("MatchingGeometry::boundary: ENTERING ");

    // boundary of geom
    return geom->getBoundary();

} // void MatchingGeometry::boundary( Geometry *boundary, const Geometry *geom )


bool MatchingGeometry::contains( const Geometry *geomA, const Geometry *geomB )
{
    qDebug("ENTERING MatchingGeometry::contain");

    return geomA->contains( geomB );

} // bool MatchingGeometry::contains( const Geometry *geomA, const Geometry *geomB )


bool MatchingGeometry::setMatch( MyGEOSGeom *geom )
{
    qDebug("ENTERING MatchingGeometry::setMatch");

    // find close geometries
    closeGeometries( geom->getGEOSGeom() );
    unsigned int csSize = closeSet.size();

    // compute buffers
    Geometry *bufferA = buffer( geom->getGEOSGeom() );
    Geometry *boundaryA = boundary( geom->getGEOSGeom() );
    Geometry *bufferBoundaryA = buffer( boundaryA );

    // find matching geometry
    for ( unsigned int i = 0; i < csSize; i++ )
    {
        const Geometry *geomB = closeSet[i].getGEOSGeom();
        Geometry *bufferB = buffer( geom->getGEOSGeom() );

        // if buffer of one contains the other it is on a good way
        if ( contains( bufferA, geomB ) && contains( bufferB, geom->getGEOSGeom() ) )
        {
            Geometry *boundaryB = boundary( geom->getGEOSGeom() );
            Geometry *bufferBoundaryB = buffer( boundaryB );

            // if boundary buffer of one contains boundary of the other -> geometries are similar
            if ( contains( bufferBoundaryA, boundaryB ) && contains( bufferBoundaryB, boundaryA ) )
            {
                geom->setMatchingGeom( &closeSet[i] );
                return true;
            }
        }

    }

    return false;

} // bool MatchingGeometry::setMatch( MyGEOSGeom *geom, bool isFirst )
