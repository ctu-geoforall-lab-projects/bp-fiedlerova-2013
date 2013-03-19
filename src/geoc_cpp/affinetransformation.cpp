#include "affinetransformation.h"

AffineTransformation::AffineTransformation()
{
    // initialize private variables
    identicPoints1 = NULL;
    identicPoints2 = NULL;
    transfSet = NULL;

} // constructor


double AffineTransformation::determinant()
{
    qDebug("AffineTransformation::determinant: ENTERING");

    // get coordinates of identical points
    double xA, yA, xB, yB, xC, yC;
    xA = identicPoints1->getX( 0 );
    yA = identicPoints1->getY( 0 );
    xB = identicPoints1->getX( 1 );
    yB = identicPoints1->getY( 1 );
    xC = identicPoints1->getX( 2 );
    yC = identicPoints1->getY( 2 );

    // compute determinant
    double det = xA*(yC - yB) + xC*(yB - yA) + xB*(yA - yC);

    return det;

} // double AffineTransformation::determinant()


void AffineTransformation::computeParameters2D( const double det, double &a1, double &a2, double &b1, double &b2)
{
    qDebug("AffineTransformation::computeParameters2D: ENTERING");

    // get coordinates of identical points in the first system
    double xA, yA, xB, yB, xC, yC;
    xA = identicPoints1->getX( 0 );
    yA = identicPoints1->getY( 0 );
    xB = identicPoints1->getX( 1 );
    yB = identicPoints1->getY( 1 );
    xC = identicPoints1->getX( 2 );
    yC = identicPoints1->getY( 2 );

    // get coordinates of identical points in the second system
    double xAA, yAA, xBB, yBB, xCC, yCC;
    xAA = identicPoints2->getX( 0 );
    yAA = identicPoints2->getY( 0 );
    xBB = identicPoints2->getX( 1 );
    yBB = identicPoints2->getY( 1 );
    xCC = identicPoints2->getX( 2 );
    yCC = identicPoints2->getY( 2 );

    // compute coordinate differences
    double dxAB = xB - xA;
    double dyAB = yB - yA;
    double dxAC = xC - xA;
    double dyAC = yC - yA;

    double dXAB = xBB - xAA;
    double dYAB = yBB - yAA;
    double dXAC = xCC - xAA;
    double dYAC = yCC - yAA;

    // compute parameters
    a1 = ( dyAB*dXAC - dyAC*dXAB )/det;
    a2 = ( dxAC*dYAB - dxAB*dYAC )/det;
    b1 = ( dxAB*dXAC - dxAC*dXAB )/det;
    b2 = ( dyAB*dYAC - dyAC*dYAB )/det;

} // bool AffineTransformation::computeParameters2D( const double det, double &a1, double &a2, double &b1, double &b2)


void AffineTransformation::transformPoint2D( double *x, double *y)
{
   qDebug("AffineTransformation::transformPoint2D: ENTERING");

    // precise tolerance
    const double TOL = 1e-12;

    // compute determinant
    double det = determinant();

    // check if determinant is not 0
    if( det < TOL )
    {
        qDebug("AffineTransformation::transformPoint2D: Determinant is 0. Identical points on the line.");
    }

    // compute parameters of transformation
    double a1, a2, b1, b2;
    computeParameters2D( det, a1, a2, b1, b2 );

    // get coordinates of first id. point
    double xA, yA;
    xA = identicPoints1->getX( 0 );
    yA = identicPoints1->getY( 0 );

    double xAA, yAA;
    xAA = identicPoints2->getX( 0 );
    yAA = identicPoints2->getY( 0 );

    // transform coordinates
    double xx = xAA + a1*(*x - xA) - b1*(*y - yA);
    double yy = yAA + b2*(*x - xA) + b2*(*y - yA);

    // set transformed coordinates
    *x = xx;
    *y = yy;

} // void AffineTransformation::transformPoint2D( double *x, double *y)


void AffineTransformation::affineTransf2D()
{
    qDebug("AffineTransformation::affineTransf2D: ENTERING");

    // get number of identical points
    unsigned int iSize1, iSize2;
    iSize1 = identicPoints2->getSize();
    iSize2 = identicPoints2->getSize();

    // check number of identical points
    if( iSize1 != 3 || iSize1 != iSize2 )
    {
        qDebug("AffineTransformation::affineTransf2D: Wrong number of identical points");
        return;
    }

    // transform geometry
    unsigned long tSize = transfSet->size();

    for ( unsigned long i = 0; i < tSize; i++ )
    {
        //CoordinateSequence * points = transfSet->at(i).getGEOSGeom()->getCoordinates();

        //unsigned int cSize = points->getSize();

        // transform each point from geometry - coordinate sequence
        //for ( unsigned int j = 0; j < cSize; j++ )
        //{
            // get point coordinates
            double x = transfSet->getX(i);
            double y = transfSet->getY(i);

            // transform point
            transformPoint2D(&x, &y);

            // setX, setY ??

        //}

        // set new geometry
        /*Geometry *g = NULL;
        editGeometry( g, points );
        transfSet->at(i).setGEOSGeom( g );*/

    }

} // void AffineTransformation::affineTransf2D()


void AffineTransformation::editGeometry( Geometry *g, CoordinateSequence *coord)
{
    qDebug("AffineTransformation::editGeometry: ENTERING");

    // edit geometry  <-- USE IT DIFFERENTLY THAN IN VERTEXSNAPPER !!!
    VertexGeometryEditorOperation myOp;

    GeometryEditor geomEdit( g->getFactory() );

    // set geometry to edited one
    geomEdit.edit( g , &myOp );

} // void AffineTransformation::editGeometry( Geometry *g, const CoordinateSequence *coord)
