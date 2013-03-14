#include "featuresnapper.h"

FeatureSnapper::FeatureSnapper()
{

}

void FeatureSnapper::snap()
{
    qDebug("FeatureSnapper::snap: ENTERING");

    newGeometry = subGeometry;

    // try to snap each feature from subject layer
    unsigned int sSize = newGeometry.size();
    unsigned int rSize = refGeometry.size();
    for ( unsigned int i = 0; i < sSize; i++ )
    {
        TGeomLayer closeFeat;
        qDebug("FeatureSnapper::snap: created empty closeFeat");

        // find close features from the ref geometry
        for ( unsigned int j = 0; j < rSize; j++ )
        {
            // check if features are close
            if( isClose( refGeometry[j], newGeometry[i] ) )
            {
                closeFeat.push_back( refGeometry[j] );
            }
        }

        // snap feature to the corresponding feature from ref layer
        if (closeFeat.size() > 0)
        {
            snapFeatures( &newGeometry[i], &closeFeat );
            qDebug("FeatureSnapper::snap: snapping to closeFeat");
        }

    }

} // void FeatureSnapper::snap()


bool FeatureSnapper::isClose(MyGEOSGeom & g1, MyGEOSGeom & g2)
{
    qDebug("FeatureSnapper::isClose: ENTERING");

    // min distance between geometries is less than tolerance
    if ( g1.getGEOSGeom()->distance( g2.getGEOSGeom() ) < tolDistance )
    {
       return true;
    }

    return false;
} // bool FeatureSnapper::isClose(MyGEOSGeom & g1, MyGEOSGeom & g2)


void FeatureSnapper::snapFeatures( MyGEOSGeom * geom, TGeomLayer *closeFeatures )
{
    qDebug("FeatureSnapper::snapFeatures: ENTERING");

    // initialize geometry matcher
    MatchingGeometry matcher;
    matcher.setGeometrySet( closeFeatures );
    matcher.setTolDistance(tolDistance);

    // find matching geometry
    if( matcher.setMatch( geom ) )
    {
        editGeometry( geom );
        qDebug("FeatureSnapper::snapFeatures: matching geometry found");
    }

} // void FeatureSnapper::snapFeatures( MyGEOSGeom * geom, const TGeomLayer *closeFeatures )


void FeatureSnapper::editGeometry( MyGEOSGeom * geom )
{
    qDebug("FeatureSnapper::editGeometry: ENTERING");

    // change geometry to the geometry of coressponding feature
    geom->setGEOSGeom( geom->getMatched()->getGEOSGeom() );
    geom->setChanged( true );

    if ( geom->getGEOSGeom()->isValid() )
    {
        qDebug("FeatureSnapper::editGeometry: geom is valid ");
    }

} // void FeatureSnapper::editGeometry( MyGEOSGeom * geom )
