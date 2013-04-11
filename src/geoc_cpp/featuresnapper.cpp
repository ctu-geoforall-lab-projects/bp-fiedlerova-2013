#include "featuresnapper.h"

namespace geoc {
namespace alg {

FeatureSnapper::FeatureSnapper()
{
    index = NULL;

} // constructor

FeatureSnapper::~FeatureSnapper()
{
    if (index) delete index;

} // destructor


void FeatureSnapper::buildIndex()
{
    // create new index
    //delete index;
    index = new STRtree();

    // add envelopes of geometries to index
    size_t gSize = refGeometry.size();
    for ( size_t i = 0; i < gSize; i++ )
    {
        const Geometry* g = refGeometry[i].getGEOSGeom();
        const Envelope* env = g->getEnvelopeInternal();
        index->insert(env, (void*)g );
    }

} // void FeatureSnapper::buildIndex()


void FeatureSnapper::snap()
{
    // build spatial index
    buildIndex();

    newGeometry = subGeometry;

    // try to snap each feature from subject layer
    unsigned int sSize = newGeometry.size();

    for ( size_t i = 0; i < sSize; i++ )
    {
        TGeomLayer closeFeat;

        // use spatial index
        vector<void*> results;
        index->query( newGeometry[i].getGEOSGeom()->getEnvelopeInternal(), results );
        size_t rSize = results.size();

        // find close features from the ref geometry
        for ( size_t j = 0; j < rSize; j++  )
        {
            // get envelope of tested feature
            Geometry *searchGeom = static_cast<Geometry*>( results[j] );
            Envelope subEnv = *( newGeometry[i].getGEOSGeom()->getEnvelopeInternal() );
            subEnv.expandBy( tolDistance ); // expand envelope with tolerance distance

            if ( subEnv.intersects( searchGeom->getEnvelopeInternal() ) )
            {
                // add coordinates from close features
                closeFeat.push_back( GEOCGeom(searchGeom) );
            }
        }

        // snap feature to the corresponding feature from ref layer
        if (closeFeat.size() > 0)
        {
            snapFeatures( &newGeometry[i], &closeFeat );
        }

    }


} // void FeatureSnapper::snap()


bool FeatureSnapper::isClose(GEOCGeom & g1, GEOCGeom & g2)
{
    // min distance between geometries is less than tolerance
    if ( g1.getGEOSGeom()->distance( g2.getGEOSGeom() ) <= tolDistance )
    {
       return true;
    }

    return false;

} // bool FeatureSnapper::isClose(GEOCGeom & g1, GEOCGeom & g2)


void FeatureSnapper::snapFeatures( GEOCGeom * geom, TGeomLayer *closeFeatures )
{
    // initialize geometry matcher
    MatchingGeometry matcher;
    matcher.setGeometrySet( closeFeatures );
    matcher.setTolDistance(tolDistance);

    // find matching geometry
    if( matcher.setMatch( geom ) )
    {
        editGeometry( geom );
    }

} // void FeatureSnapper::snapFeatures( GEOCGeom * geom, const TGeomLayer *closeFeatures )


void FeatureSnapper::editGeometry( GEOCGeom * geom )
{
    // change geometry to the geometry of coressponding feature
    geom->setGEOSGeom( geom->getMatched() );
    geom->setChanged( true );

    // create and set geometry editor
    /*FeatureGeometryEditorOperation myOp;
    myOp.setTolDistance( tolDistance );
    myOp.setGeometry(geom);

    GeometryEditor geomEdit( geom->getGEOSGeom()->getFactory() );

    // set geometry to edited one
    geom->setGEOSGeom( geomEdit.edit( geom->getGEOSGeom() , &myOp ) );

    // check if geometry was changed
    geom->setChanged( myOp.isChanged() );*/

    // check validity
    if ( !geom->getGEOSGeom()->isValid() )
    {
        qDebug("FeatureSnapper::editGeometry: geom is not valid ");
        invalids.push_back(geom->getFeatureId());
    }

} // void FeatureSnapper::editGeometry( GEOCGeom * geom )

} //namespace geoc
} //namespace alg
