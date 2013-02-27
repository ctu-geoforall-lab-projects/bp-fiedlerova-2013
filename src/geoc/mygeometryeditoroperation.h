#ifndef MYGEOMETRYEDITOROPERATION_H
#define MYGEOMETRYEDITOROPERATION_H

// local includes
#include "geoc.h"

using namespace std;

/** Class for editing geometry by coordinate sequence. */

class MyGeometryEditorOperation//: public CoordinateOperation
{

    //using CoordinateOperation::edit;

public:

    /// Constructor
    MyGeometryEditorOperation( GEOSCoordSequence *coord ){ mCoord = coord; }

    /** Set private mCoord to given coord.
     */
    void setCoordSeq( GEOSCoordSequence *coord ) { mCoord = coord; }

    /** Virtual function for editing geometry according to mCoord.
     @param coordinates Not important.
     @param geom Geometry to be edited.
     */
    GEOSCoordSequence* edit(const GEOSCoordSequence *coordinates, const GEOSGeometry *geom)
    {
        /*long cSize = coordinates->size();

        vector<Coordinate> *vc = new vector<Coordinate>( cSize );
        // copy coordinates and change
        for ( unsigned int i=0; i < cSize; ++i )
        {

            Coordinate coord = mCoord->getAt(i);
            (*vc)[i] = coord;

        }

        CoordinateSequence *newCoords = geom->getFactory()->getCoordinateSequenceFactory()->create(vc);
        */
        return mCoord;
    }


private:

   GEOSCoordSequence *mCoord;

};


#endif // MYGEOMETRYEDITOROPERATION_H
