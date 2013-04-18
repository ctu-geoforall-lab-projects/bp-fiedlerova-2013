#ifndef GEOC_H
#define GEOC_H

namespace geoc {
    class SortByX;
    class SortByY;
    namespace geo {
        class GEOCGeom;
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
    namespace tin {
        class Triangle;
        class Triangulation;
    }
}

#include "geocgeom.h"
#include "triangle.h"
#include "sort.h"
#include "vertexsnapper.h"
#include "vertexgeometryeditoroperation.h"
#include "affinetransformation.h"
#include "matchinggeometry.h"
#include "triangulation.h"
#include "coveragealignment.h"
#include "aligngeometryeditoroperation.h"
#include "linematcher.h"
#include "geometrycorrectionoperation.h"

#include  <QtGui>

#include <cmath>

#ifndef M_PI
#define M_PI       3.14159265358979323846
#endif

#endif // GEOC_H
