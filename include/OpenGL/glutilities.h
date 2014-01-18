#ifndef GLUTILITIES_H
#define GLUTILITIES_H

#include "../phgutils.h"
#include "../Geometry/point.hpp"
#include "../Geometry/geometryutils.hpp"

namespace PhGUtils
{
    void drawBoundingBox(const Point3d& pMin,
                         const Point3d& pMax);

    void drawLine(const Point3d& p1,
                  const Point3d& p2,
                  double lineWidth = 1.0);

    void drawSphere(const Point3d& center,
                    double radius);

    void drawArrow(const Point3d& source,
                   const Point3d& dir);

    void drawRectangle(Point3d corners[4], double lineWidth = 1.0);

    void fillRectangle(Point3d corners[4]);
}

#endif // GLUTILITIES_H
