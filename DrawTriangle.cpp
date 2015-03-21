#include <cmath>
#include "DrawTriangle.h"
/*
struct point2D{
    double x;
    double y;
};
*/
/*
struct point{
    double x;
    double y;
    double z;
};
*/

vector<point> volume;

void brezenhem2D(int coord_11, int coord_12, int coord_21, int coord_22, vector<point2D>& vec)
{
    vec.clear();

    point2D curr;

	//Defining of line direction
	int dx;
	int dy;
	if((coord_21 - coord_11) != 0)
	{
	    if(coord_21 - coord_11 > 0)
			dx = 1;
		else
			dx = -1;
	}
	else
		dx = 0;

	if(coord_22 - coord_12 != 0)
	{
	    if(coord_22 - coord_12  > 0)
			dy = 1;
		else
			dy = -1;
	}
	else
		dy = 0;
	//-------------------------

	int deltax = abs(coord_21 - coord_11);
    int deltay = abs(coord_22 - coord_12);
	double error = 0;
	double deltaerr;
    
    if(deltax != 0 && deltay != 0)
    {
	    if(deltax > deltay)
            deltaerr = double(deltay) / double(deltax);
	    else
		    deltaerr = double(deltax) / double(deltay);
    }
    else
    {
        deltaerr = 0;
    }
    
	int x = coord_11;
	int y = coord_12;

    if(deltax != 0 || deltay != 0)
    {
	  if(deltax > deltay){
		while(x != coord_21)
	    {
            curr.x = x;
            curr.y = y;
            vec.push_back(curr);
            error += deltaerr;
            if(error >= 0.5){
                y += dy;
                error -= 1;
		    }
		    x += dx;
	    }
	    curr.x = coord_21;
        curr.y = coord_22;
        vec.push_back(curr);
      }
	  else{
		while(y != coord_22)
	    {
            curr.x = x;
            curr.y = y;
            vec.push_back(curr);
            error += deltaerr;
            if(error >= 0.5){
                x += dx;
                error -= 1;
		    }
		    y += dy;
	    }
	    curr.x = coord_21;
        curr.y = coord_22;
        vec.push_back(curr);
      }
    }
    else
    {
        curr.x = coord_21;
        curr.y = coord_22;
        vec.push_back(curr);
    }
}

void brezenhem3D(point p1, point p2, vector<point>& pnt)
{
    vector<point2D> pnt1;
    vector<point2D> pnt2;

    p1.x = int(p1.x);
    p1.y = int(p1.y);
    p1.z = int(p1.z);
    p2.x = int(p2.x);
    p2.y = int(p2.y);
    p2.z = int(p2.z);

    pnt.clear();
    point curr;

    if(abs(p2.x - p1.x) > abs(p2.y - p1.y))
    {
        if(abs(p2.x - p1.x) > abs(p2.z - p1.z))
        {
            brezenhem2D(p1.x, p1.z, p2.x, p2.z, pnt1);
            brezenhem2D(p1.x, p1.y, p2.x, p2.y, pnt2);
            for(int i = 0; i < pnt1.size(); i++)
            {
                curr.x = pnt1[i].x;
                curr.y = pnt2[i].y;
                curr.z = pnt1[i].y;
                pnt.push_back(curr);
            }
        }
        else
        {
            brezenhem2D(p1.z, p1.x, p2.z, p2.x, pnt1);
            brezenhem2D(p1.z, p1.y, p2.z, p2.y, pnt2);
            for(int i = 0; i < pnt1.size(); i++)
            {
                curr.x = pnt1[i].y;
                curr.y = pnt2[i].y;
                curr.z = pnt1[i].x;
                pnt.push_back(curr);
            }
        }
        
    }
    else
    {
        if(abs(p2.y - p1.y) > abs(p2.z - p1.z))
        {
            brezenhem2D(p1.y, p1.z, p2.y, p2.z, pnt1);
            brezenhem2D(p1.y, p1.x, p2.y, p2.x, pnt2);
            for(int i = 0; i < pnt1.size(); i++)
            {
                curr.x = pnt2[i].y;
                curr.y = pnt1[i].x;
                curr.z = pnt1[i].y;
                pnt.push_back(curr);
            }
        }
        else
        {
            brezenhem2D(p1.z, p1.x, p2.z, p2.x, pnt1);
            brezenhem2D(p1.z, p1.y, p2.z, p2.y, pnt2);
            for(int i = 0; i < pnt1.size(); i++)
            {
                curr.x = pnt1[i].y;
                curr.y = pnt2[i].y;
                curr.z = pnt1[i].x;
                pnt.push_back(curr);
            }
        }
        
    }
}

void drawTriangl(point p1, point p2, point p3, vector<point>& buff){
    
    buff.clear();
    
    vector<point> base;
    vector<point> line;

    brezenhem3D(p2, p3, base);

    for(int i = 0; i < base.size(); i++)
    {
        brezenhem3D(p1, base[i], line);
        for(int j = 0; j < line.size(); j++)
            buff.push_back(line[j]);
    }
}
