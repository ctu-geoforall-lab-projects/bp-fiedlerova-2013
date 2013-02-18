#ifndef MYGEOMETRYEDITOROPERATION_H
#define MYGEOMETRYEDITOROPERATION_H

// GEOS includes
#include <geos/geom/CoordinateSequence.h>
#include <geos/geom/Geometry.h>
#include <geos/geom/util/CoordinateOperation.h>
#include <geos/geom/CoordinateSequenceFactory.h>

// local includes
#include "geoc.h"

#include <QtGui>

using namespace std;
using namespace geos;
using namespace geos::geom;
using namespace geos::geom::util;

/** Class for editing geometry by coordinate sequence. */

class MyGeometryEditorOperation: public CoordinateOperation
{

    using CoordinateOperation::edit;

public:

    /// Constructor
    MyGeometryEditorOperation( CoordinateSequence *coord ){ mCoord = coord; }

    /** Set private mCoord to given coord.
     */
    void setCoordSeq( CoordinateSequence *coord ) { mCoord = coord; }

    /** Virtual function for editing geometry according to mCoord.
     @param coordinates Not important.
     @param geom Geometry to be edited.
     */
    CoordinateSequence* edit(const CoordinateSequence *coordinates, const Geometry *geom)
    { //qDebug("entering EDIT"); coordinates = mCoord;
        qDebug("setting coord"); //return mCoord;

       long cSize = coordinates->size();

        vector<Coordinate> *vc = new vector<Coordinate>( cSize );
        // copy coordinates and change
        for ( unsigned int i=0; i < cSize; ++i )
        {

            Coordinate coord=mCoord->getAt(i); //(0.0*k, 100.0*k );//

            (*vc)[i] = coord;

        }

        CoordinateSequence *newCoords = geom->getFactory()->getCoordinateSequenceFactory()->create(vc);

        return newCoords;
    }


private:

    CoordinateSequence *mCoord;

};


#endif // MYGEOMETRYEDITOROPERATION_H
