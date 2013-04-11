#ifndef SORT_H
#define SORT_H

#include "geoc.h"

#include <cmath>

namespace geoc {

/** Class for sort Coordinate by X */

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


/** Class for sort Coordinate by Y */

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
