/***************************************************************************
    geoc.h

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

#ifndef GEOC_H
#define GEOC_H

namespace geoc {
    class SortByX;
    class SortByY;
    namespace geo {
        class GEOCGeom;
    }
    namespace tin {
        class Triangle;
        class Triangulation;
    }
    namespace idx {
        class SpatialIndexBuilder;
    }
    namespace edit {
        class AlignGeometryEditorOperation;
        class VertexGeometryEditorOperation;
        class FeatureGeometryEditorOperation;
        class AffineTransformation;
        class GeometryCorrectionOperation;
    }
    namespace alg {
        class VertexSnapper;
        class MatchingGeometry;
        class CoverageAlignment;
        class LineMatcher;
    }
}

#include "geocgeom.h"
#include "triangle.h"
#include "sort.h"
#include "spatialindexbuilder.h"
#include "vertexsnapper.h"
#include "vertexgeometryeditoroperation.h"
#include "affinetransformation.h"
#include "matchinggeometry.h"
#include "triangulation.h"
#include "coveragealignment.h"
#include "aligngeometryeditoroperation.h"
#include "linematcher.h"
#include "geometrycorrectionoperation.h"

#include <cmath>

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#endif // GEOC_H
