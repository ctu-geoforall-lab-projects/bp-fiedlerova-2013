// local includes
#include "vertexsnapper.h"

VertexSnapper::VertexSnapper()
{
    sIndex = NULL;
    tolDistance = 0;

} // constructor


VertexSnapper::~VertexSnapper()
{
    if (sIndex) delete sIndex;

} // destructor


void VertexSnapper::buildIndex()
{
    // create new index
    //delete index;
    sIndex = new STRtree();

    // add envelopes of geometries to index
    size_t gSize = refGeometry.size();
    for ( size_t i = 0; i < gSize; i++ )
    {
        const Geometry* g = refGeometry[i].getGEOSGeom();
        const Envelope* env = g->getEnvelopeInternal();
        sIndex->insert(env, (void*)g );
    }

} // void VertexSnapper::buildIndex()


void VertexSnapper::snap()
{
    // build spatial index
    buildIndex();

    newGeometry = subGeometry;

    for ( size_t i = 0; i < subGeometry.size(); i++ )
    {
        // find close features from the reference layer
        CoordinateSequence *closeCoord = new CoordinateArraySequence();

        // use spatial index
        vector<void*> results;
        sIndex->query( subGeometry[i].getGEOSGeom()->getEnvelopeInternal(), results );
        size_t rSize = results.size();

        // get close coordinates
        for ( size_t j = 0; j < rSize; j++ )
        {
            // get envelope of tested feature
            const Geometry *searchGeom = static_cast<const Geometry*>( results[j] );
            Envelope subEnv = *( subGeometry[i].getGEOSGeom()->getEnvelopeInternal() );
            subEnv.expandBy( tolDistance ); // expand envelope with tolerance distance

            // features are close if their envelopes intersects
            if ( subEnv.intersects( searchGeom->getEnvelopeInternal() ) )
            {
                // add coordinates from close features
                closeCoord->add( searchGeom->getCoordinates(), true, true );
            }

        }

        // snap vertex if there are close points
        if ( closeCoord->getSize() > 0 )
        {
            MyGEOSGeom newGeom = subGeometry[i];
            snapVertices( &newGeom, closeCoord );
            newGeometry[i] = newGeom;
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

    // create and set geometry editor
    VertexGeometryEditorOperation myOp;
    myOp.setCloseCoordSeq( closeCoord );
    myOp.setTolDistance( tolDistance );

    GeometryEditor geomEdit( geom->getGEOSGeom()->getFactory() );

    // set geometry to edited one
    geom->setGEOSGeom( geomEdit.edit( geom->getGEOSGeom() , &myOp ) );

    // check if geometry was changed
    geom->setChanged( myOp.isChanged() );

    // check validity
    if( !geom->getGEOSGeom()->isValid() )
    {
        qDebug("VertexSnapper::snapVertices: Geom is not valid.");
        invalids.push_back(geom->getFeatureId());
    }

} // void VertexSnapper::snapVertices(MyGEOSGeom *geom, CoordinateSequence *closeCoord)
