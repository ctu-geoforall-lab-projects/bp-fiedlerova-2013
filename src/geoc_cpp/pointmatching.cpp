#include "pointmatching.h"

PointMatching::PointMatching()
{  
    tolDistance = 1;
}


TGeomLayer PointMatching::findCandidates( Geometry *point, short vid )
{
    // vector of close points
    TGeomLayer candidates;

    // size of vector with points
    unsigned int size = pointGeometry2.size();

    if ( vid == 1)
    {
        size = pointGeometry1.size();
    }

    // find close points
    for (unsigned long i = 0; i < size; i++)
    {
        double dist = 0.0;

        if(vid == 1)
        {
            dist = point->distance( pointGeometry1[i].getGEOSGeom() );

            // add close point to candidates
            if ( dist < tolDistance )
            {
                candidates.push_back( pointGeometry1[i]);
            }
        }

        else
        {
            dist = point->distance( pointGeometry2[i].getGEOSGeom() );

            // add close point to candidates
            if ( dist < tolDistance )
            {
                candidates.push_back( pointGeometry2[i]);
            }
        }

    }

    return candidates;

} // TGeomLayer PointMatching::findCandidates( Coordinate *point )


int PointMatching::getSector( const Coordinate * point1, const Coordinate * point2 )
{
    // angle between X an point1-point2
    double rad = Angle::angle( *point1, *point2 );

    // find right sector
    if ( rad >= -M_PI && rad < 3*M_PI/4 )
    {
        return 5;
    }
    else if ( rad < -M_PI/2 )
    {
        return 6;
    }
    else if ( rad < - M_PI/4 )
    {
        return 7;
    }
    else if ( rad < 0 )
    {
        return 8;
    }
    else if ( rad < M_PI/4 )
    {
        return 1;
    }
    else if ( rad < M_PI/2 )
    {
        return 2;
    }
    else if ( rad < 3*M_PI/4 )
    {
        return 3;
    }
    else if ( rad < M_PI )
    {
        return 4;
    }

    // return 0 in case of nonsense
    return 0;

} // int PointMatching::getSector( Coordinate * point1, Coordinate * point2 )


double PointMatching::getLengthDiff( CoordinateSequence * cs1, CoordinateSequence * cs2)
{
    // test length of segments
    if ( cs1->size() != 2 || cs2->size() != 2 )
    {
        return -1;
    }

    // lengths of segments
    double length1 = cs1->getAt(0).distance( cs1->getAt(1) );
    double length2 = cs2->getAt(0).distance( cs2->getAt(1) );

    // difference of lengths
    double diff = std::abs( length1 - length2 );

    return diff;

} //double gPointMatching::etLengthDiff( CoordinateSequence * cs1, CoordinateSequence * cs2)


vector<int> PointMatching::findSectors( const Coordinate * point, TGeomLayer closes)
{
    // get size of close points
    unsigned int cSize = closes.size();
    std::vector<int> sectors(cSize);

    for ( unsigned int i = 0; i < cSize; i++ )
    {
        // compute sector
        int sec = getSector( point, closes[i].getGEOSGeom()->getCoordinate() );
        sectors[i] = sec;
    }

    return sectors;

} // vector<int> PointMatching::findSectors( Coordinate * point, TGeomLayer closes)


bool PointMatching::compareSectors( vector<int> sectors1, vector<int> sectors2 )
{
    // pozn. nelze jednodusse porovnat, musi se spocitat, kolikrat je tam ktery sektor a pak porovnat tato cisla
    // musi se take vzit v potaz rozdil v delkach obu vektoru

    return true;

} // bool compareSectors( vector<int> sectors1, vector<int> sectors2 )


void PointMatching::setMatchingPoint( MyGEOSGeom * point, short vid )
{
    // match point
    MyGEOSGeom *match = new MyGEOSGeom();

    // find close points from point's and candidate's layer
    int pid = 1;
    if ( vid == 1)
    {
        pid = 2;
    }
    TGeomLayer points = findCandidates( point->getGEOSGeom(), pid );
    TGeomLayer candidates = findCandidates( point->getGEOSGeom(), vid );

    // get sectors from point's and candidate's layer
    vector<int> sectorsP = findSectors( point->getGEOSGeom()->getCoordinate(), points );

    unsigned int cSize = candidates.size();
    for (unsigned int i = 0; i < cSize; i++)
    {
        vector<int> sectorsC = findSectors( candidates[i].getGEOSGeom()->getCoordinate(), candidates );

        // compare sectors
        bool sector = compareSectors( sectorsP, sectorsC );

        // erase candidate if sectors are not similar
        if ( !sector )
        {
            candidates.erase( candidates.begin() + i );
            i--;
            cSize--;
        }
    }

    // zde by melo byt porovnani delek segmentu a pouziti nejake funkce lengthDiffs, ktera by secetla a zprumerovala
    // delky vsech odpovidajicich si segmentu

    // set matching point
    point->setMatchingGeom( match );

} // void PointMatching::setMatchingPoint( MyGEOSGeom * point, short vid )
