#include "geometrycorrectionoperation.h"

namespace geoc {
namespace edit {

CoordinateSequence* GeometryCorrectionOperation::edit(const CoordinateSequence *, const Geometry *geom )
{
    CoordinateSequence* coord = geom->getCoordinates();

    // remove repeated points
    CoordinateSequence *coordRem = CoordinateSequence::removeRepeatedPoints(coord);

    if( coordRem->size() > 3 )
    {
        coord = coordRem;
    }

    // repair crossing
    if (coord->size() > 3)
    {
        removeCrosses(coord);

        // remove dead branches
        removeDeadBranch(coord);
    }

    return coord;

}  // CoordinateSequence* GeometryCorrectionOperation::edit(const CoordinateSequence *, const Geometry *geom )


void GeometryCorrectionOperation::removeDeadBranch( CoordinateSequence *line )
{

    for ( size_t i = 0; i < line->size()-2; i++ )
    {
        // line has a dead branch, if points i and i+2 are equal
        if ( (abs(line->getX(i) - line->getX(i+2)) < 1e-12) && (abs(line->getY(i) - line->getY(i+2)) < 1e-12) )
        {
            if ( line->size() > 4 )
            {
                line->deleteAt(i+1);
                i--;
            }
            else
            {
                break;
            }
        }
    }// for

} // void GeometryCorrectionOperation::removeDeadBranch( CoordinateSequence *line )


void GeometryCorrectionOperation::removeCrosses( CoordinateSequence *line )
{
    for ( size_t i = 0; i < line->size()-1; i++ )
    {
        for (size_t j = i+2; j < line->size()-1; j++)
        {
            vector<Coordinate> vc1;
            vc1.push_back(line->getAt(i));
            vc1.push_back(line->getAt(i+1));
            vector<Coordinate> vc2;
            vc2.push_back(line->getAt(j));
            vc2.push_back(line->getAt(j+1));

            CoordinateSequence *c1 = new CoordinateArraySequence(&vc1);
            CoordinateSequence *c2 = new CoordinateArraySequence(&vc2);

            GeometryFactory f;
            if ( f.createLineString(c1)->crosses( f.createLineString(c2) ) )
            {
                // change segment orientation
                size_t l = j;
                size_t k = i+1;
                while( k < l )
                {
                    //if ( k < l )
                    {
                        Coordinate temp = line->getAt(k);
                        line->setAt(line->getAt(l), k);
                        line->setAt(temp, l);
                        l--;
                    }

                    k++;
                    l--;

                } // while

                i=0; // order of segments was changed, check it again



            } // if
        } // for
    }// for

} // void GeometryCorrectionOperation::removeCrosses( CoordinateSequence *line )


} //namespace geoc
} //namespace edit
