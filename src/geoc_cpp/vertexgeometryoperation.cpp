#include "vertexgeometryeditoroperation.h"

CoordinateSequence* VertexGeometryEditorOperation::edit(const CoordinateSequence *coordinates, const Geometry *geom )
{
    CoordinateSequence* coord = geom->getCoordinates();

    // find closest point from closeCoord
    for ( unsigned int i = 0; i < coord->getSize(); i++)
    {
        // minimal distance
        double minDist = tolDistance;
        unsigned int indMin = 0;
        bool isMin = false;

        for ( unsigned int j = 0; j < closeCoord->getSize(); j++ )
        {
            // compute distance between two tested points
            double dist = coord->getAt(i).distance( closeCoord->getAt(j) );

            if( dist < minDist )
            {
                minDist = dist;
                indMin = j;
                isMin = true;
            }

        }

        // set new coordinate to the closest point if there is some
        if ( isMin )
        {
            coord->setAt( closeCoord->getAt(indMin), i );
            changed = true;
        }

    }

    return coord;
}
