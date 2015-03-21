#ifndef ALGO_H
#define ALGO_H

#include <vector>
#include "VectOperators.h"

bool isInVector(std::vector<int> vect, int val);
bool isInVector(std::vector<point> vect, point p);

int defSidesNum(int arg);
int defVertNum(int arg);

point normalize(point vect, int RADIUS);

void defVertRel(int divNum, int RADIUS, std::vector<point>& vert, std::vector<std::vector<int> >& vertRel);
void defVertRel(int divNum, int RADIUS, std::vector<point>& vert, std::vector<std::vector<int> >& vertRel);

void devideSphere(int iterNum, int RADIUS, std::vector<point>& vert, std::vector<std::vector<int> >& vertRel);

void initSphere(int divNum, int RADIUS,/* point center, */ std::vector<point>& vert, std::vector<std::vector<int> >& vertRel);

void defMedians(std::vector<point> vert, std::vector<std::vector<int> >& vertRel, std::vector<point>& median);

void sortRel(std::vector<point> vert, std::vector<std::vector<int> >& vertRel);

void reSort(std::vector<point> vert, std::vector<point> norm, std::vector<std::vector<int> >& vertRel);

void defNorms(int RADIUS, std::vector<point> vert, std::vector<std::vector<int> > vertRel, std::vector<point>& norm);

void defOrt(std::vector<point> vert, std::vector<point> median, std::vector<point> norm, std::vector<point>& ort);

void defTang(std::vector<point> vert, std::vector<point> median, std::vector<point> ort, std::vector<point>& tang);

void initAlgo(int divNum/*, point cent*/, int RADIUS, std::vector<point>& vert, std::vector<std::vector<int> >& vertRel,
    std::vector<point>& ort, std::vector<point>& tang);

point defNormal(point p0, point p1, point p2, point p3, point p4, point p5, point p6);

#endif
