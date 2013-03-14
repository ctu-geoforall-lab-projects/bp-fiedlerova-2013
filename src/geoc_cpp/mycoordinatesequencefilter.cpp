#include "mycoordinatesequencefilter.h"

void MyCoordinateSequenceFilter::filter_ro(const CoordinateSequence &, size_t)
{
}


void MyCoordinateSequenceFilter::filter_rw(CoordinateSequence &coords, size_t i)
{
    // set coordinate from seq to coords
    if(seq.size() == coords.size())
    {
        coords.setAt( seq[i], i);
        done = true;
    }


}


bool MyCoordinateSequenceFilter::isDone() const
{
    return done;
}


bool MyCoordinateSequenceFilter::isGeometryChanged() const
{
    return true;
}
