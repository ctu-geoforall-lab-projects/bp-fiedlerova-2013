/***************************************************************************
    sort.h

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

#ifndef SORT_H
#define SORT_H

#include "geoc.h"

#include <cmath>

namespace geoc {

/** Class for sort Coordinate by X. */

class SortByX
{
    public:

        bool operator() (const Coordinate& p1, const Coordinate& p2) const
        {
            const double TOL = 1e-14;

            if ( std::abs( p1.x-p2.x ) < TOL)
            {
                return p1.y < p2.y;
            }

            return p1.x < p2.x;
        }
};


/** Class for sort Coordinate by Y. */

class SortByY
{
    public:

        bool operator() (const Coordinate& p1, const Coordinate& p2) const
        {
            const double TOL = 1e-14;

            if ( std::abs( p1.y-p2.y ) < TOL)
            {
                return p1.x < p2.x;
            }

            return p1.y < p2.y;
        }
};


} //namespace geoc

#endif // SORT_H
