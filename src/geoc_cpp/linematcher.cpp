/***************************************************************************
    linematcher.cpp

    GEOC - GEOS Conflation library

    ---------------------
    begin                : April 2013
    copyright            : (C) 2013 by Tereza Fiedlerová
    email                : tfiedlerova dot at gmail dot com
 ***************************************************************************
 *                                                                         *
 *   This is free software; you can redistribute it and/or modify it       *
 *   under the terms of the GNU General Public License as published by     *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 ***************************************************************************/

#include "linematcher.h"

namespace geoc{
namespace alg{

LineMatcher::LineMatcher()
{
    sIndex = NULL;
    tolDistance = 0;
    matchTolerance = 0.7;
    diffMax = 0;

} // constructor


LineMatcher::~LineMatcher()
{
    if (sIndex)
    {
        delete sIndex;
    }

} // destructor


void LineMatcher::match()
{
    newGeometry = subGeometry;

    // build spatial index
    sIndex = SpatialIndexBuilder::buildIndex(&refGeometry);

    // set diffMax
    longestLine();

    for ( size_t i = 0; i < subGeometry.size(); i++ )
    {
        // find close features from the reference layer
        vector<CoordinateSequence*> closeLines;

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
                CoordinateSequence *cs = geos::geom::CoordinateSequence::removeRepeatedPoints(
                                         searchGeom->getCoordinates() );
                closeLines.push_back( cs );
            }

        }

        // process line if there are close lines
        if ( closeLines.size() > 0 )
        {
            GEOCGeom newGeom = subGeometry[i];
            matchLine( &newGeom, closeLines );
            newGeometry[i] = newGeom;

            // check validity
            if( !newGeometry[i].getGEOSGeom()->isValid() )
            {
                qDebug("LineMatcher::match: Geom is not valid.");
                invalids.push_back(newGeometry[i].getFeatureId());
            }

        } // if

    } // for

} // void LineMatcher::match()


void LineMatcher::matchLine( GEOCGeom * gline, vector<CoordinateSequence *> & closeLines )
{

    const CoordinateSequence * line = geos::geom::CoordinateSequence::removeRepeatedPoints(
                                      gline->getGEOSGeom()->getCoordinates() );
    const GeometryFactory *gf = gline->getGEOSGeom()->getFactory();
    CoordinateSequence * newLine = new CoordinateArraySequence();
    vector<Geometry *> vc;
    size_t lastId = 0;

    // for each segment
    for (size_t i = 0; i < line->size()-1; i++ )
    {
        // segment
        CoordinateSequence * segment = new CoordinateArraySequence(2);
        segment->setAt( line->getAt(i), 0 );
        segment->setAt( line->getAt(i+1), 1 );

        // find matching segment
        CoordinateSequence * match = matchSegment( segment, closeLines );

        // get mean segment
        if ( match )
        {

            CoordinateSequence * result = meanSegment( segment, match );

            // check if there are following segments
            if ( lastId == i )
            {
               newLine->add( result, false, true );
            }
            else
            {
                vc.push_back( gf->createLineString( newLine) );
                newLine = result;
            }

            lastId = i+1;

        }

        delete segment;
    }

    // add also last part of line
    vc.push_back( gf->createLineString(newLine) );

    // create new line from matched segments
    gline->setGEOSGeom( gf->createMultiLineString( vc ) );
    gline->setChanged( true );

    delete newLine;

} // void LineMatcher::matchLine( GEOCGeom * line, vector<CoordinateSequence *> & closeLines )


CoordinateSequence * LineMatcher::matchSegment( CoordinateSequence* segment, vector<CoordinateSequence *> & closeLines )
{
    // find closest segment from closeLines

    // minimal distance
    double matchCoef = 0;
    size_t indMin = 0;
    int segIdx = 0;

    for ( size_t j = 0; j < closeLines.size(); j++ )
    {
        // compute distance between two tested points
        int idx;
        double coef = closestSegment( segment, closeLines[j], idx);

        if( coef > matchCoef )
        {
            matchCoef = coef;
            indMin = j;
            segIdx = idx;
        }

    }

    // compute mean of segments
    if ( matchCoef > matchTolerance )
    {
        CoordinateSequence* lseg = new CoordinateArraySequence(2);

        if ( segIdx < 0 )
        {
            segIdx *= -1;
            lseg->setAt( closeLines[indMin]->getAt(segIdx-1), 1 );
            lseg->setAt( closeLines[indMin]->getAt(segIdx), 0 );
        }
        else
        {
            lseg->setAt( closeLines[indMin]->getAt(segIdx-1), 0 );
            lseg->setAt( closeLines[indMin]->getAt(segIdx), 1 );
        }

        return lseg;
    }

    return NULL;

} // CoordinateSequence * LineMatcher::matchSegment( CoordinateSequence* segment, vector<CoordinateSequence *> & closeLines )


double LineMatcher::closestSegment( CoordinateSequence * seg, CoordinateSequence *line, int & idx )
{
    double matchCoef = 0;

    // return closest segment from line
    for ( size_t i = 1; i < line->size(); i++ )
    {
        CoordinateSequence* lseg = new CoordinateArraySequence( 2 );
        lseg->setAt( line->getAt(i-1), 0 );
        lseg->setAt( line->getAt(i), 1 );

        // check line orientation
        bool dir = checkDirection( seg, lseg );

        // compute distance criterium - if lines are close enough
        Coordinate a = Coordinate( (seg->getX(0)+seg->getX(1))/2.0, (seg->getY(0)+seg->getY(1))/2.0 );
        Coordinate b = Coordinate( (lseg->getX(0)+lseg->getX(1))/2.0, (lseg->getY(0)+lseg->getY(1))/2.0 );
        double d = a.distance(b);

        // compute match only if segments are close enough
        if (d < tolDistance )
        {
            double coef = similarity( seg, lseg );

            if ( coef > matchCoef )
            {
                matchCoef = coef;
                idx = i;
                if ( !dir )
                {
                    idx = -i;
                }
            }
        } // if

        delete lseg;

    } // for

    return matchCoef;

} // CoordinateSequence* LineMatcher::closestSegment( CoordinateSequence * seg, CoordinateSequence *line )


bool LineMatcher::checkDirection( CoordinateSequence * s1, CoordinateSequence * s2 )
{
    double angle = segmentAngle(s1, s2);

    if( angle > M_PI/2)
    {
        geos::geom::CoordinateSequence::reverse(s2);
        return false;
    }

    return true;

} // void LineMatcher::checkDirection( CoordinateSequence *line, vector<CoordinateSequence *> & closeLines )


double LineMatcher::segmentLength(const CoordinateSequence *seg ) const
{
    return seg->getAt(0).distance(seg->getAt(1));

} // double LineMatcher::segmentLength( CoordinateSequence *seg )


double LineMatcher::segmentAngle( const CoordinateSequence *s1, const CoordinateSequence *s2 ) const
{
    const double PRECISION = 1e-12;

    double ux = s1->getX(1)-s1->getX(0);
    double uy = s1->getY(1)-s1->getY(0);
    double vx = s2->getX(1)-s2->getX(0);
    double vy = s2->getY(1)-s2->getY(0);

    // compute size of vectors
    double u = sqrt( (ux*ux+uy*uy) );
    double v = sqrt( (vx*vx+vy*vy) );

    // angle between segments
    double nom = u*v;
    double angle;

    // null vector
    if ( abs(nom) < PRECISION )
    {
        angle = INFINITY;
    }
    // compute deviation
    else
    {
        double cosinus = (ux*vx+uy*vy)/nom;
        if ( abs(cosinus-1) < PRECISION ) // cos is 1
        {
            angle = 0;
        }
        else if ( abs(cosinus+1) < PRECISION ) // cos is -1
        {
            angle = M_PI;
        }
        else
        {
            angle = abs( acos( cosinus ) );
        }
    }

    return angle;

} // double LineMatcher::segmentAngle( CoordinateSequence *s1, CoordinateSequence *s2 )


double LineMatcher::similarity( const CoordinateSequence * c1, const CoordinateSequence * c2) const
{
    // angle
    double dev = segmentAngle( c1, c2 );

    // length difference
    double length1 = segmentLength(c1);
    double length2 = segmentLength(c2);
    double diff = abs( length1 - length2 );

    // distance criterium
    double dist1 = c1->getAt(0).distance( c2->getAt(0) );
    double dist2 = c1->getAt(1).distance( c2->getAt(1) );
    double dist = (dist1+dist2)/2.0;

    // max and min differences and deviations in dataset - set from data, not like this
    double devMax = M_PI/2;
    double devMin = 0;
    //double diffMax = max( length1, length2 );//+tolDistance;
    double diffMin = 0;
    double distMax = tolDistance;
    double distMin = 0;

    // match criterium
    double s1 = ( diffMax-diff )/( diffMax-diffMin );
    double s2 = ( devMax-dev )/( devMax-devMin );
    double s3 = ( distMax-dist )/( distMax-distMin );  

    double smin;
    if ( (s1 > matchTolerance) && (s2 > matchTolerance) && (s3 > matchTolerance))
    {
        smin = (s1+s2+s3)/3.0;  // distance criterium is the most important
    }
    else
    {
        return 0;
    }

    // return in % how much they match
    return smin;

} // double LineMatcher::similarity(CoordinateSequence * c1, CoordinateSequence * c2)


CoordinateSequence* LineMatcher::meanSegment( CoordinateSequence * s1, CoordinateSequence *s2 )
{
    // return mean of s1 and s2
    CoordinateSequence *cs = new CoordinateArraySequence(2);
    double x1 = ( s1->getX(0)+s2->getX(0) )*0.5;
    double y1 = ( s1->getY(0)+s2->getY(0) )*0.5;
    double x2 = ( s1->getX(1)+s2->getX(1) )*0.5;
    double y2 = ( s1->getY(1)+s2->getY(1) )*0.5;

    cs->setAt( Coordinate(x1,y1), 0 );
    cs->setAt( Coordinate(x2,y2), 1 );

    return cs;

} // CoordinateSequence* LineMatcher::meanSegment( CoordinateSequence * s1, CoordinateSequence *s2 )


void LineMatcher::longestLine()
{

    for ( size_t i = 0; i < subGeometry.size(); i++ )
    {
        CoordinateSequence *cs = subGeometry[i].getGEOSGeom()->getCoordinates();
        for ( size_t j = 0; j < cs->size()-1; j++ )
        {
            double dl = cs->getAt(j).distance(cs->getAt(j+1));
            if ( dl > diffMax )
            {
                diffMax = dl;
            }
        }
    }

} // double LineMatcher::longestLine()

} // namespace alg
} // namespace geoc
