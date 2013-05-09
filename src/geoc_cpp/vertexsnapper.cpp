/***************************************************************************
    vertexsnapper.cpp

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

// local includes
#include "vertexsnapper.h"

namespace geoc {
namespace alg {

VertexSnapper::VertexSnapper()
{
    sIndex = NULL;
    tolDistance = 0;
    correct = false;

} // constructor


VertexSnapper::~VertexSnapper()
{
    if (sIndex)
    {
        delete sIndex;
    }

} // destructor

#ifdef WITHOUT_SPIDX // for testing

void VertexSnapper::snap()
{

    newGeometry = subGeometry;

    if ((refGeometry.size() == 0) || (subGeometry.size() == 0) )
    {
        return;
    }

    for ( unsigned int i = 0; i < subGeometry.size(); i++ )
    {
        // find close features from the reference layer
        CoordinateSequence *closeCoord = new CoordinateArraySequence();

        for ( unsigned int j = 0; j < refGeometry.size(); j++)
        {
            // check if features from sub and ref are close
            bool close = subGeometry[i].getGEOSGeom()->distance( refGeometry[j].getGEOSGeom() ) < tolDistance;
            if (close)
            {
                // add close coordinates
                closeCoord->add(refGeometry[j].getGEOSGeom()->getCoordinates(), true, true );
            }

        }

        // snap vertex if there are close points
        if ( closeCoord->size() > 0 )
        {
            GEOCGeom newGeom = subGeometry[i];
            snapVertices( &newGeom, closeCoord );
            newGeometry[i] = newGeom;
        }

        if (closeCoord)
        {
            delete closeCoord;
        }

    }

} // void VertexSnapper::snap()

#else

void VertexSnapper::snap()
{

    newGeometry = subGeometry;

    if ((refGeometry.size() == 0) || (subGeometry.size() == 0) )
    {
        return;
    }

    // build spatial index
    sIndex = SpatialIndexBuilder::buildIndex(&refGeometry);


    for ( size_t i = 0; i < subGeometry.size(); i++ )
    {
        // find close features from the reference layer
        CoordinateSequence *closeCoord = new CoordinateArraySequence();

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
                CoordinateSequence *cs = searchGeom->getCoordinates();
                closeCoord->add( cs, true, true );
                delete cs;
            }
        }

        // snap vertex if there are close points
        if ( closeCoord->size() > 0 )
        {
            GEOCGeom newGeom = subGeometry[i];
            snapVertices( &newGeom, closeCoord );
            newGeometry[i] = newGeom;
        }

        if (closeCoord)
        {
            delete closeCoord;
        }

    }

} // void VertexSnapper::snap()

#endif


void VertexSnapper::snapVertices(GEOCGeom *geom, CoordinateSequence *closeCoord)
{

    // create and set geometry editor
    VertexGeometryEditorOperation myOp;
    myOp.setCloseCoordSeq( closeCoord );
    myOp.setTolDistance( tolDistance );

    GeometryEditor geomEdit( geom->getGEOSGeom()->getFactory() );

    // set geometry to edited one
    geom->setGEOSGeom( geomEdit.edit( geom->getGEOSGeom() , &myOp ) );

    // check if geometry was changed
    geom->setChanged( myOp.isChanged() );

    // repair geometry if wanted
    if (correct)
    {
        GeometryCorrectionOperation::repair(geom);
    }

    // check validity
    if( !geom->getGEOSGeom()->isValid() )
    {
        invalids.push_back(geom->getFeatureId());
    }


} // void VertexSnapper::snapVertices(GEOCGeom *geom, CoordinateSequence *closeCoord)


} //namespace geoc
} //namespace alg
