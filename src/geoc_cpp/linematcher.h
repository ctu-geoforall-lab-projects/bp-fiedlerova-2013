/***************************************************************************
    linematcher.h

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

#ifndef LINEMATCHER_H
#define LINEMATCHER_H

// local includes
#include "geoc.h"

using namespace geoc;
using namespace geoc::geo;
using namespace geoc::alg;
using namespace geoc::idx;


namespace geoc{
namespace alg{

/** Class for matching line feature layers. */

class LineMatcher
{

public:

    /** Default constructor.
      */
    LineMatcher();

    /** Destructor.
      */
    ~LineMatcher();

    /** Set reference layer geometries.
      * @param ref Vector of geometries in reference layer.
      */
    void setRefGeometry(TGeomLayer & ref){ refGeometry = ref; }

    /** Set subject layer geometries.
      * @param sub Vector of geometries in subject layer.
      */
    void setSubGeometry(TGeomLayer & sub){ subGeometry = sub; }

    /** Set distance tolerance for matching.
      * @param tol Maximal distance for matching.
      */
    void setTolDistance(double tol){ tolDistance = tol; }

    /** Set matching tolerance.
      * @param tol Minimal tolerance for match.
      */
    void setMatchTolerance(double tol){ matchTolerance = tol; }

    /** Get new geometry.
      * @return New layer geometry.
      */
    TGeomLayer & getNewGeometry(){ return newGeometry; }

    /** Match line segments.
     */
    void match();

    /** Get list of invalid geometries.
      */
    vector<int> getInvalidGeometries() { return invalids; }

private:

    TGeomLayer refGeometry;
    TGeomLayer subGeometry;
    TGeomLayer newGeometry;
    SpatialIndex* sIndex;
    vector<int> invalids;
    double tolDistance;
    double matchTolerance;
    double diffMax;

    /** Find matching segment to each segment of given line.
      * @param line Pointer to the line.
      * @param closeLines Vector with with close lines from the reference layer.
      */
    void matchLine( GEOCGeom * line, vector<CoordinateSequence *> & closeLines );

    /** Find matching segment.
      * @param geom Pointer to the segment coordinates.
      * @param closeLines Vector with with close lines from the reference layer.
      * @return Matching segment.
      */
    CoordinateSequence  * matchSegment( CoordinateSequence  * seg, vector<CoordinateSequence *> & closeLines );

    /** Find closest segment from line and return match criterium value
      * @param seg CoordinateSequence with points of tested segment.
      * @param line CoordinateSequence with points of tested line.
      * @param idx Index of matching segment.
      */
    double closestSegment( CoordinateSequence * seg, CoordinateSequence *line, int & idx);

    /** Test whether two segments are within tolerance distance and angle tolerance.
      * @param c1 CoordinateSequence with points of the first tested segment.
      * @param c2 CoordinateSequence with points of the  second tested segment.
      * @return How much given segments are matched in percents.
      */
    double similarity( const CoordinateSequence * c1, const CoordinateSequence  * c2) const;

    /** Compute segment length
      * @param segment CoordinateSequence with two points of segment.
      * @return length of segment
      */
    double segmentLength( const CoordinateSequence *seg ) const;

    /** Compute angle between segments
      * @param s1 CoordinateSequence with two points of first segment.
      * @param s1 CoordinateSequence with two points of first segment.
      * @return length of segment
      */
    double segmentAngle( const CoordinateSequence *s1, const CoordinateSequence *s2 ) const;

    /** Compute mean segment.
      * @param s1 Segment from subject layer.
      * @param s2 Segment from reference layer.
      * @return Geometry of mean segment.
      */
    CoordinateSequence * meanSegment( CoordinateSequence  * s1, CoordinateSequence  *s2 );

    /** Check line orientation and change its direction if necessary
      * @param s1 Segment from subject layer.
      * @param s2 Segment from reference layer.
      */
    bool checkDirection( CoordinateSequence * s1, CoordinateSequence * s2 );

    /** The longest line in dataset.
      */
    void longestLine();

};

} // namespace alg
} // namespace geoc

#endif // LINEMATCHER_H
