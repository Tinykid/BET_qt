#ifndef DEFORMATION_H
#define DEFORMATION_H

#include "Algo.h"
#include <vector>

int minI(point p1, point p2, int w, int h, int n, unsigned char** volume);
int maxI(point p1, point p2, int w, int h, int n, unsigned char** volume);

void deformation(int RADIUS, int w, int h, int n, std::vector<point>& vert,  std::vector<point>& ort,  std::vector<point>& tang, unsigned char** volume);

void deformationD(int RADIUS, int w, int h, int n, std::vector<point>& vert, std::vector<std::vector<int> > vertRel,  std::vector<point>& ort,  std::vector<point>& tang, unsigned char** volume);

void deformationComp1(int RADIUS, int w, int h, int n, std::vector<point>& vert,  std::vector<point>& ort,  std::vector<point>& tang, unsigned char** volume);

void normUpdate(std::vector<point>& vert,  std::vector<std::vector<int> > vertRel);

#endif
