#include "VectOperators.h"
#include "math.h"

double dist(point a, point b)
{
    return sqrt(pow(a.x - b.x, 2) + pow(a.y - b.y, 2) + pow(a.z - b.z, 2));
}

point middlepoint(point a, point b)
{
    point c;
	c.x = (a.x + b.x) / 2;
	c.y = (a.y + b.y) / 2;
	c.z = (a.z + b.z) / 2;

	return c;
}

point operator*(point a, point b)
{
	point c;
	c.x = a.y * b.z - a.z * b.y;
	c.y = a.z * b.x - a.x * b.z;
	c.z = a.x * b.y - a.y * b.x;
	return c;
}

point operator*(point a, double b)
{
	point c;
	c.x = a.y * b;
	c.y = a.z * b;
	c.z = a.x * b;
	return c;
}

point operator*(double b, point a)
{
	point c;
	c.x = a.y * b;
	c.y = a.z * b;
	c.z = a.x * b;
	return c;
}

point operator+(point a, point b)
{
	point c;
	c.x = a.x + b.x;
	c.y = a.y + b.y;
	c.z = a.z + b.z;
	return c;
}

point operator/(point a, double b)
{
	point c;
	c.x = a.x / b;
	c.y = a.y / b;
	c.z = a.z / b;
	return c;
}

point operator-(point a, point b)
{
	point c;
	c.x = a.x - b.x;
	c.y = a.y - b.y;
	c.z = a.z - b.z;
	return c;
}

bool operator!=(point a, point b)
{
	if((a.x != b.x) || (a.y != b.y) || (a.z != b.z))
        return 1;
	else
		return 0;
}

bool operator==(point a, point b)
{
	if(a.x == b.x && a.y == b.y && a.z == b.z)
        return 1;
	else
		return 0;
}

void operator>>(point a, point &b)
{
	b.x = a.x;
    b.y = a.y;
    b.z = a.z;
}

void operator>>(double a, point &b)
{
	b.x = a;
    b.y = a;
    b.z = a;
}

double dist(point vect)
{
	return sqrt(pow(vect.x, 2) + pow(vect.y, 2) + pow(vect.z, 2));
}

