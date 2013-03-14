#ifndef AFFINETRANSFORMATION_H
#define AFFINETRANSFORMATION_H

// local includes
#include "geoc.h"

using namespace std;

/** Class providing funstions for affine transformation */

class AffineTransformation
{

public:

    /** Default constructor.
      */
    AffineTransformation();

    /** Set identical points - vertices of triangle in the first system.
      */
    void setIdenticPoints1( const CoordinateSequence * points ){ identicPoints1 = points; }

    /** Set identical points - vertices of triangle in the second system.
      */
    void setIdenticPoints2( const CoordinateSequence * points ){ identicPoints2 = points; }

    /** Set geometry of layer to be transformed.
      */
    void setTransfSet( CoordinateSequence * set){ transfSet = set; }

    /** Computes determinant ( 2 x area of identical points triangle ).
      */
    double determinant();

    /** Computes elements/parameters of 2D affine transformation
      */
    void computeParameters2D( const double det, double &a1, double &a2, double &b1, double &b2);

    /** Transform only one point with given coordinates.
      */
    void transformPoint2D( double *x, double *y);

    /** Computes 2D affine transformation on set of transformed point.
      */
    void affineTransf2D();

    /** Edit geometry according to given coordinates.
      * @param g Geometry to be edited.
      * @param coord Coordinate sequence with new coordinates for geometry.
      */
    void editGeometry( Geometry *g, const CoordinateSequence *coord);

    /** Computes elements of 3D affine transformation
      */
    //void computeParameters3D( const double det, double &a1, double &a2, double &b1, double &b2, ...);

    /** Computes 3D affine transformation.
      */
    //void affineTransf3D(); // Do I need it?


private:

    const CoordinateSequence *identicPoints1; // type? // for 2D transformation 3 identical points
    const CoordinateSequence *identicPoints2;
    CoordinateSequence *transfSet; // set of points to be transformed  // Maybe just a vector of points??????

};

#endif // AFFINETRANSFORMATION_H
