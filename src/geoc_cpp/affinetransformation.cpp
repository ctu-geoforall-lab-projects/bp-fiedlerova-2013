#include "affinetransformation.h"

AffineTransformation::AffineTransformation()
{
    // initialize private variables
    identicPoints1 = NULL;
    identicPoints2 = NULL;

} // constructor


double AffineTransformation::determinant()
{

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


void AffineTransformation::transformPoint2D( Coordinate *point )
{    

    // check number of identic points
    if ((identicPoints1->size() < 3) || (identicPoints1->size() != identicPoints2->size()) )
    {
        qDebug("AffineTransformation::transformPoint2D: Wrong number of identical points.");
        return;
    }

    // precise tolerance
    const double TOL = 1e-14;

    // compute determinant
    double det = determinant();

    // check if determinant is not 0
    if( abs(det) < TOL )
    {
        qDebug("AffineTransformation::transformPoint2D: Determinant is 0. Identical points on the line.");
        return;
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
    double xx = xAA + a1*(point->x - xA) - b1*(point->y - yA);
    double yy = yAA + b2*(point->x - xA) + a2*(point->y - yA);

    // set transformed coordinates
    point->x = xx;
    point->y = yy;

} // void AffineTransformation::transformPoint2D( Coordinate *point )
