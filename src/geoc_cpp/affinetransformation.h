/***************************************************************************
    affinetransformation.h

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

#ifndef AFFINETRANSFORMATION_H
#define AFFINETRANSFORMATION_H

// local includes
#include "geoc.h"

using namespace std;

using namespace geoc;
using namespace geoc::geo;
using namespace geoc::alg;
using namespace geoc::edit;


namespace geoc {
namespace edit {

/** Class providing functions for affine transformation of point from system 1 to system 2. */

class AffineTransformation
{

public:

    /** Default constructor.
      */
    AffineTransformation();

    /** Destructor
      */
    ~AffineTransformation();

    /** Set identical points - vertices of triangle in the first system.
      * @param points Identical points from original system.
      */
    void setIdenticPoints1( const CoordinateSequence * points ){ identicPoints1 = points; }

    /** Set identical points - vertices of triangle in the second system.
      * @param points Identical points from new system.
      */
    void setIdenticPoints2( const CoordinateSequence * points ){ identicPoints2 = points; }

    /** Transform point from system 1 to system 2.
      * @param point Point to be transformed.
      */
    void transformPoint2D( Coordinate *point );


private:

    const CoordinateSequence *identicPoints1; // for 2D transformation - 3 identical points
    const CoordinateSequence *identicPoints2;

    /** Computes determinant ( 2 x area of identical points triangle ).
      */
    double determinant();

    /** Computes elements/parameters of 2D affine transformation
      */
    void computeParameters2D( const double det, double &a1, double &a2, double &b1, double &b2);

};

} //namespace geoc
} //namespace edit

#endif // AFFINETRANSFORMATION_H
