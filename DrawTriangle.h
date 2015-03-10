#ifndef DrawTriangle_H
#define DrawTriangle_H

#include "VectOperators.h"
#include <vector>

using namespace std;

struct point2D{
    double x;
    double y;
};

void brezenhem2D(int coord_11, int coord_12, int coord_21, int coord_22, vector<point2D>& vec);

void brezenhem3D(point p1, point p2, vector<point>& pnt);

void drawTriangl(point p1, point p2, point p3, vector<point>& buff);

#endif


