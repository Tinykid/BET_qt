#ifndef VectOperators_H
#define VectOperators_H

struct point
{
	double x;
	double y;
	double z;
};

double dist(point a);
double dist(point a, point b);

point middlepoint(point a, point b);

point operator*(point a, point b);
point operator+(point a, point b);
point operator-(point a, point b);
point operator/(point a, double b);
point operator*(point a, double b);
point operator*(double b, point a);
bool operator!=(point a, point b);
bool operator==(point a, point b);
void operator>>(point a, point &b); //assign a to b
void operator>>(double a, point &b); //assign a to b

#endif