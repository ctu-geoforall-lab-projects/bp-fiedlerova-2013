#include "aligngeometryeditoroperation.h"

#include<iostream>

namespace geoc {
namespace edit {

AlignGeometryEditorOperation::~AlignGeometryEditorOperation()
{
    if (sIndex) delete sIndex;
    /*if (idPoints1) delete idPoints1;
    if (idPoints2) delete idPoints2;
    if (tin) delete tin;*/
}

CoordinateSequence* AlignGeometryEditorOperation::edit(const CoordinateSequence *, const Geometry *g )
{
    CoordinateSequence* coord = g->getCoordinates();
    size_t cSize = coord->size();

    // transform each point from geometry
    for ( size_t i = 0; i < cSize; i++ )
    {
        Coordinate *point = new Coordinate( coord->getX(i), coord->getY(i) );

        if ( findIdPoints( point ) )
        {
            AffineTransformation at;
            at.setIdenticPoints1( idPoints1 );
            at.setIdenticPoints2( idPoints2 );
            at.transformPoint2D( point );
            changed = true;
        }

        coord->setAt( *point, i );

        delete point;

    }

    return coord;

} // CoordinateSequence* AlignGeometryEditorOperation::edit(const CoordinateSequence *, const Geometry *g )


bool AlignGeometryEditorOperation::findIdPoints( Coordinate *point )
{
    // use spatial index
    vector<void*> results;
    Envelope * searchEnv = new Envelope( *point );
    sIndex->query( searchEnv, results );
    size_t rSize = results.size();

    // build point geometry
    GeometryFactory f;
    Geometry * p = f.createPoint( *point );

    // test all close triangles
    for ( size_t i = 0; i < rSize; i++ )
    {
        Geometry *searchGeom = static_cast<Geometry*>( results[i] );
        Geometry *g = f.createGeometry(searchGeom);
        Triangle * t = static_cast<Triangle *>( searchGeom->getUserData() );

        // check if point is inside triangle
        if ( g->intersects( p ) )
        {
            idPoints1 = t->getTriangle();
            idPoints2 = t->getCorrespondingTriangle();

            delete searchEnv;
            f.destroyGeometry(p);
            f.destroyGeometry(g);
            return true;
        }

        f.destroyGeometry(g);

    }

    delete searchEnv;
    f.destroyGeometry(p);
    return false;

} // bool AlignGeometryEditorOperation::findIdPoints( Coordinate *point )


void AlignGeometryEditorOperation::buildIndex()
{
    // create new index
    //delete sIndex;
    sIndex = new STRtree();

    // add envelopes of geometries to index
    size_t gSize = tin->size();
    for ( size_t i = 0; i < gSize; i++ )
    {
        Geometry* g = tin->at(i).getTriangleGeom();
        g->setUserData( (void*)(&tin->at(i)) );
        const Envelope* env = g->getEnvelopeInternal();

        sIndex->insert(env, (void*)g );
    }

} // void AlignGeometryEditorOperation::buildIndex()


} //namespace geoc
} //namespace edit
