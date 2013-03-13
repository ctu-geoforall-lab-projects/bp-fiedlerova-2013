#include "matchinggeometry.h"

MatchingGeometry::MatchingGeometry()
{
    tolDistance = 1;
    geometrySet1 = NULL;
    geometrySet2 = NULL;
}

void MatchingGeometry::setGeometrySet( TGeomLayer * geometrySet, bool isFirst )
{
    qDebug("MatchingGeometry::setGeometrySet: ENTERING ");

    // set the right geometrySet
    if ( isFirst )
    {
        geometrySet1 = geometrySet;
    }
    else
    {
        geometrySet2 = geometrySet;
    }

} // void MatchingGeometry::setGeometrySet( TGeomLayer geometrySet, bool isFirst )


void  MatchingGeometry::closeGeometries( TGeomLayer * closeSet, const Geometry *geom, bool isFirst )
{
    qDebug("MatchingGeometry::isClose: ENTERING ");

    // set set :)
    TGeomLayer *geometrySet = geometrySet2;

    if( isFirst )
    {
        geometrySet = geometrySet1;
    }

    // find close geometries
    closeSet->clear();
    unsigned long size = geometrySet->size();
    for ( unsigned long i = 0; i < size; i++ )
    {
        // test wether geometry from set is within given tolerance distance from tested geometry
        if ( isClose( geometrySet->at(i).getGEOSGeom(), geom ) )
        {
            closeSet->push_back( geometrySet->at(i) );
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


void MatchingGeometry::buffer( Geometry *buffer, const Geometry *geom )
{
    qDebug("MatchingGeometry::buffer: ENTERING ");

    // buffer
    buffer = geom->buffer( tolDistance );

} // void MatchingGeometry::buffer( Geometry *buffer, const Geometry *geom )


void MatchingGeometry::boundary( Geometry *boundary, const Geometry *geom )
{
    qDebug("MatchingGeometry::boundary: ENTERING ");

    // boundary of geom
    boundary = geom->getBoundary();

} // void MatchingGeometry::boundary( Geometry *boundary, const Geometry *geom )


bool MatchingGeometry::contains( const Geometry *geomA, const Geometry *geomB )
{
    qDebug("ENTERING MatchingGeometry::contain");

    return geomA->contains( geomB );

} // bool MatchingGeometry::contains( const Geometry *geomA, const Geometry *geomB )


bool MatchingGeometry::setMatch( MyGEOSGeom *geom, bool isFirst )
{
    qDebug("ENTERING MatchingGeometry::setMatch");

    // find close geometries
    TGeomLayer *closeG = NULL;
    closeGeometries( closeG, geom->getGEOSGeom(), isFirst );
    unsigned int size = closeG->size();

    // compute buffers
    Geometry *bufferA = NULL;
    Geometry *boundaryA = NULL;
    Geometry *bufferBoundaryA = NULL;
    buffer( bufferA, geom->getGEOSGeom() );
    boundary( boundaryA, geom->getGEOSGeom() );
    buffer( bufferBoundaryA, boundaryA );

    // find matching geometry
    for ( unsigned int i = 0; i < size; i++ )
    {
        const Geometry *geomB = closeG->at(i).getGEOSGeom();
        Geometry *bufferB = NULL;
        buffer( bufferB, geom->getGEOSGeom() );

        // if buffer of one contains the other it is on a good way
        if ( contains( bufferA, geomB ) && contains( bufferB, geom->getGEOSGeom() ) )
        {
            Geometry *boundaryB = NULL;
            Geometry *bufferBoundaryB = NULL;
            boundary( boundaryB, geom->getGEOSGeom() );
            buffer( bufferBoundaryB, boundaryB );

            // if boundary buffer of one contains boundary of the other -> geometries are similar
            if ( contains( bufferBoundaryA, boundaryB ) && contains( bufferBoundaryB, boundaryA ) )
            {
                geom->setMatchingGeom( &(closeG->at(i)) );
                return true;
            }
        }

    }

    return false;

} // bool MatchingGeometry::setMatch( MyGEOSGeom *geom, bool isFirst )
