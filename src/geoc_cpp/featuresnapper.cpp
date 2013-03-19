#include "featuresnapper.h"

FeatureSnapper::FeatureSnapper()
{

}

void FeatureSnapper::snap()
{
    newGeometry = subGeometry;

    // try to snap each feature from subject layer
    //unsigned int sSize = newGeometry.size();
    //unsigned int rSize = refGeometry.size();
    TGeomLayer::iterator newIt;
    TGeomLayer::iterator refIt;

    //for ( unsigned int i = 0; i < sSize; i++ )
    for (newIt = newGeometry.begin(); newIt != newGeometry.end(); newIt++ )
    {
        TGeomLayer closeFeat;

        // find close features from the ref geometry
        //for ( unsigned int j = 0; j < rSize; j++ )
        for (refIt = refGeometry.begin(); refIt != refGeometry.end(); refIt++ )
        {
            // check if features are close
            if( isClose( *refIt, *newIt ) )// refGeometry[j], newGeometry[i] ) )
            {
                closeFeat.push_back( *refIt ); //refGeometry[j] );
            }
        }

        // snap feature to the corresponding feature from ref layer
        if (closeFeat.size() > 0)
        {
            snapFeatures( &*newIt, &closeFeat ); //&newGeometry[i], &closeFeat );
        }

    }

} // void FeatureSnapper::snap()


bool FeatureSnapper::isClose(MyGEOSGeom & g1, MyGEOSGeom & g2)
{
    // min distance between geometries is less than tolerance
    if ( g1.getGEOSGeom()->distance( g2.getGEOSGeom() ) <= tolDistance )
    {
       return true;
    }

    return false;
} // bool FeatureSnapper::isClose(MyGEOSGeom & g1, MyGEOSGeom & g2)


void FeatureSnapper::snapFeatures( MyGEOSGeom * geom, TGeomLayer *closeFeatures )
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

} // void FeatureSnapper::snapFeatures( MyGEOSGeom * geom, const TGeomLayer *closeFeatures )


void FeatureSnapper::editGeometry( MyGEOSGeom * geom )
{
    // change geometry to the geometry of coressponding feature
    geom->setGEOSGeom( geom->getMatched()->getGEOSGeom() );
    geom->setChanged( true );

    if ( geom->getGEOSGeom()->isValid() )
    {
        qDebug("FeatureSnapper::editGeometry: geom is valid ");
    }

} // void FeatureSnapper::editGeometry( MyGEOSGeom * geom )
