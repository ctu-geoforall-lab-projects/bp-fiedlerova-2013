#include "triangle.h"

namespace geoc {
namespace tin {

Triangle::Triangle()
{
    triangle = NULL;
    correspondingT = NULL;
    triangleGeom = NULL;
}

Triangle::~Triangle()
{
    /*if (triangle) delete triangle;
    if (correspondingT) delete correspondingT;
    if( triangleGeom) delete triangleGeom;*/
}

} //namespace geoc
} //namespace tin
