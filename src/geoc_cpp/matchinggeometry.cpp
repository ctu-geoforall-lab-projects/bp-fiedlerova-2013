#include "matchinggeometry.h"

namespace geoc {
namespace alg {

MatchingGeometry::MatchingGeometry()
{
    sIndex = NULL;
    tolDistance = 1;
    geometrySet = NULL;

} // constructor

MatchingGeometry::~MatchingGeometry()
{
    if(sIndex) delete sIndex;

} //destructor


void MatchingGeometry::setGeometrySet( TGeomLayer * geomSet)
{
    // set the right geometrySet
    geometrySet = geomSet;

} // void MatchingGeometry::setGeometrySet( TGeomLayer geometrySet, bool isFirst )


void  MatchingGeometry::closeGeometries( const Geometry *geom )
{

    // create spatial index
    sIndex = SpatialIndexBuilder::buildIndex(geometrySet);

    // reset close geometries
    closeSet.clear();

    // use spatial index
    vector<void*> results;
    sIndex->query( geom->getEnvelopeInternal(), results );
    size_t rSize = results.size();

    // get close geometries
    for ( size_t j = 0; j < rSize; j++ )
    {
        // get envelope of tested feature
        Geometry *searchGeom = static_cast<Geometry*>( results[j] );
        Envelope subEnv = *( geom->getEnvelopeInternal() );
        //subEnv.expandBy(tolDistance); // expand envelope with tolerance distance - not necessary in this case

        if ( subEnv.intersects( searchGeom->getEnvelopeInternal() ) )
        {
            // add geometry to close features
            closeSet.push_back( searchGeom );
        }

    }

} // void  MatchingGeometry::closeGeometries( TGeomLayer * closeSet, const Geometry *geom, bool isFirst )


bool MatchingGeometry::isClose( const Geometry * g1, const Geometry * g2 )
{

    double dist = g1->distance( g2 );

    // test distance between g1 and g2 and tolerance distance
    if( dist <= tolDistance )
    {
        return true;
    }

    return false;

} // bool MatchingGeometry::isClose( const Geometry * g1, const Geometry * g2 )


Geometry* MatchingGeometry::buffer( Geometry *geom )
{
    // buffer
    return geom->buffer( tolDistance );

} // void MatchingGeometry::buffer( Geometry *buffer, const Geometry *geom )


Geometry* MatchingGeometry::boundary( Geometry *geom )
{
    // boundary of geom
    return geom->getBoundary();

} // void MatchingGeometry::boundary( Geometry *boundary, const Geometry *geom )


bool MatchingGeometry::contains( const Geometry *geomA, const Geometry *geomB )
{

    return geomA->contains( geomB );

} // bool MatchingGeometry::contains( const Geometry *geomA, const Geometry *geomB )


bool MatchingGeometry::setMatch( GEOCGeom *geom )
{

    // find close geometries
    closeGeometries( geom->getGEOSGeom() );
    unsigned int csSize = closeSet.size();

    // compute buffers
    Geometry *bufferA = buffer( geom->getGEOSGeom() );
    Geometry *boundaryA = boundary( geom->getGEOSGeom() );
    Geometry *bufferBoundaryA = buffer( boundaryA );

    GeometryFactory f;

    // find matching geometry
    for ( unsigned int i = 0; i < csSize; i++ )
    {
        const Geometry *geomB = closeSet[i];
        Geometry *bufferB = buffer( geom->getGEOSGeom() );

        // if buffer of one contains the other it is on a good way
        if ( contains( bufferA, geomB ) && contains( bufferB, geom->getGEOSGeom() ) )
        {
            Geometry *boundaryB = boundary( geom->getGEOSGeom() );
            Geometry *bufferBoundaryB = buffer( boundaryB );
            // if boundary buffer of one contains boundary of the other -> geometries are similar
            if ( contains( bufferBoundaryA, boundaryB ) && contains( bufferBoundaryB, boundaryA ) )
            {
                //geom->setChanged(true);
                geom->setMatchingGeom( closeSet[i] );  // FINDS ONLY FIRST MATCHING GEOMETRIES, WHAT ABOUT ELSE????????????

                // for clear memory
                f.destroyGeometry(bufferA);
                f.destroyGeometry(bufferB);
                f.destroyGeometry(boundaryA);
                f.destroyGeometry(boundaryB);
                f.destroyGeometry(bufferBoundaryA);
                f.destroyGeometry(bufferBoundaryB);
                return true;
            }

            // for clear memory
            f.destroyGeometry(boundaryB);
            f.destroyGeometry(bufferBoundaryB);

        }

        // for clear memory;
        f.destroyGeometry(bufferB);

    }

    // for clear memory;
    f.destroyGeometry(bufferA);
    f.destroyGeometry(boundaryA);
    f.destroyGeometry(bufferBoundaryA);


    return false;

} // bool MatchingGeometry::setMatch( GEOCGeom *geom, bool isFirst )

} // namespace alg
} // namespace geoc
