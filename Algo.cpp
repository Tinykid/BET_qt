#include "Algo.h"
#include <vector>
#include <iostream>
//#include "VectOperators.h"

using namespace std;

bool isInVector(vector<int> vect, int val)
{
    for(int i = 0; i < vect.size(); i++)
	    if(vect[i] == val)
			return true;

	return false;
}

bool isInVector(vector<point> vect, point p)
{
    for(int i = 0; i < vect.size(); i++)
	    if(vect[i] == p)
			return true;

	return false;
}

int defSidesNum(int arg)
{
    if(arg == 0)
        return 30;
    else
        return 4 * defSidesNum(arg - 1);
}

int defVertNum(int arg)
{
    if(arg == 0)
        return 12;
    else
        return defVertNum(arg - 1) + defSidesNum(arg - 1);
}

point normalize(point vect, int RADIUS)
{
    point res;
    
    res.x = RADIUS * vect.x / dist(vect);
    res.y = RADIUS * vect.y / dist(vect);
    res.z = RADIUS * vect.z / dist(vect);

    return res;
}


void defVertRel(int divNum, int RADIUS, vector<point>& vert, vector<vector<int>>& vertRel)
{
	//Distance between neigbours (depends on devNum)
    double vertDist;
    
    double a;
    a = (4 / sqrt(10 + 2 * sqrt(double(5)))) * RADIUS;
	vertDist = a / pow(2, divNum);

	for(int i = 0; i < vert.size(); i++)
    {
        vertRel[i].clear();
	    for(int j = 0; j < vert.size(); j++)
		    if((dist(vert[i], vert[j]) >= 0.95 * vertDist) && (dist(vert[i], vert[j]) <= 1.05 * vertDist))
				vertRel[i].push_back(j); 
    }
}

void devideSphere(int iterNum, int RADIUS, vector<point>& vert, vector<vector<int>>& vertRel){

    int sz = vert.size();

    defVertRel(iterNum, RADIUS, vert, vertRel);
	
    for(int i = 0; i < sz; i++)
        for(int j = 0; j < vertRel[i].size(); j++)
            if(!isInVector(vert, middlepoint(vert[i], vert[vertRel[i][j]])))
                vert.push_back(middlepoint(vert[i], vert[vertRel[i][j]]));
}

void initSphere(int divNum, int RADIUS,/* point center, */ vector<point>& vert, vector<vector<int>>& vertRel){

	#define X .525731112119133606 
    #define Z .850650808352039932 
 
	//Positions of vertices
    float vdata[12][3] = { 
        {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z}, 
        {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X}, 
        {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0} 
    }; 

    vert.clear();
    point vrt;

    for(int i = 0; i < 12; i++)
    {
        vrt.x = RADIUS * vdata[i][0];
        vrt.y = RADIUS * vdata[i][1];
        vrt.z = RADIUS * vdata[i][2];
        vert.push_back(vrt);
    }
    
    if(divNum != 0)
    {
        for(int i = 0; i < divNum; i++)
            devideSphere(i, RADIUS, vert, vertRel);

        defVertRel(divNum, RADIUS, vert, vertRel);

        for(int i = 12; i < vert.size(); i++)
            vert[i] = normalize(vert[i], RADIUS);
    }
    
}

void defMedians(vector<point> vert, vector<vector<int>>& vertRel, vector<point>& median)
{
    median.clear();

    point m;

    for(int i = 0; i < vert.size(); i++)
    {
        0 >> m;

        for(int j = 0; j < vertRel[i].size(); j++)
            m = m + vert[vertRel[i][j]];
        m = m / vertRel[i].size();

        m = m - vert[i];

        median.push_back(m);
    }
}

/*/Drawing all triangles arround i-th vertex 
  
  Indexes of neighbours are defined in vertRel[i]

  1. Pick any of them vertRel[i][0]. Put it to set<int>.
  2. Define one of 2 common neigbours. ...to set<>
  3. Define common neighbour of last vertex in set (or better vector<>)

*/

//TODO: change to object field or find corresponded function

vector<int> sorted;

void sortRel(vector<point> vert, vector<vector<int>>& vertRel)
{
    for(int i = 0; i < vert.size(); i++)
    {
        sorted.clear();
        //pick first vertex
        sorted.push_back(vertRel[i][0]);
        
        for(int k = 0; k < vertRel[i].size() - 2; k++)
            for(int j = 0; j < 5; j++)
                if(!isInVector(sorted, vertRel[i][j]) && isInVector(vertRel[sorted[sorted.size() - 1]], vertRel[i][j]))
                {
                    sorted.push_back(vertRel[i][j]);
                    break;
                }

        for(int j = 0; j < sorted.size(); j++)
            if(!isInVector(sorted, vertRel[i][j]))
                sorted.push_back(vertRel[i][j]);

        for(int j = 0; j < sorted.size(); j++)
            vertRel[i][j] = sorted[j];
    }
}

// for check direction of normals
void reSort(vector<point> vert, vector<point> norm, vector<vector<int>>& vertRel)
{  
    sorted.clear();
    int sz;

    for(int i = 0; i < vert.size(); i++)
        if(dist(vert[i] + norm[i]) <= dist(vert[i]))      //if same direction
        {
            sz = i;
            
            sz = vertRel[i].size();
            
            for(int j = 0; j < sz; j++)
                sorted.push_back(vertRel[i][j]);

            vertRel[i].clear();

            vertRel[i].push_back(sorted[0]);

            for(int j = sz - 1; j > 0; j--)
                vertRel[i].push_back(sorted[j]);

            sorted.clear();
            
        }
    
}

point defNormal(point p0, point p1, point p2, point p3, point p4, point p5)
{
	//p1, ..., p5 must be presented like vectors (in bases)	
    return (p1 - p0) * (p2 - p0) + (p2 - p0) * (p3 - p0) + (p3 - p0) * (p4 - p0) + (p4 - p0) * (p5 - p0) + (p5 - p0) * (p1 - p0);
}

/*
point defNormal(point p1, point p2, point p3, point p4, point p5, point p6)
{
	//p1, ..., p6 must be presented like vectors (in bases)
	return p1 * p2 + p2 * p3 + p3 * p4 + p4 * p5 + p5 * p6 + p6 * p1;
}
*/

point defNormal(point p0, point p1, point p2, point p3, point p4, point p5, point p6)
{
    //p1, ..., p6 must be presented like vectors (in bases)
    return (p1 - p0) * (p2 - p0) + (p2 - p0) * (p3 - p0) + (p3 - p0) * (p4 - p0) + (p4 - p0) * (p5 - p0) + (p5 - p0) * (p6 - p0) + (p6 - p0) * (p1 - p0);
}

//Used for moving. Not for lighteting.
void defNorms(int RADIUS, vector<point> vert, vector<vector<int>> vertRel, vector<point>& norm)
{
    norm.clear();

	for(int i = 0; i < vert.size(); i++)
	{		
		if(vertRel[i].size() == 5)
            norm.push_back(0.2 * normalize(defNormal(vert[i], vert[vertRel[i][0]], vert[vertRel[i][1]], vert[vertRel[i][2]],
			    vert[vertRel[i][3]], vert[vertRel[i][4]]), RADIUS));

		if(vertRel[i].size() == 6)
            norm.push_back(0.2 * normalize(defNormal(vert[i], vert[vertRel[i][0]], vert[vertRel[i][1]], vert[vertRel[i][2]],
			    vert[vertRel[i][3]], vert[vertRel[i][4]], vert[vertRel[i][5]]), RADIUS));	

        if(!(vertRel[i].size() == 5 || vertRel[i].size() == 6))
            cout << "defNorms(): for " << i << "-th vertex vertRel[i].size() = " << vertRel[i].size() << "\n"; 
	}
}

// input: line n(by 2 points) and point M
// output: point H on n where MH ortogonal to n
point defH(point p1, point p2, point m)
{
    point h;
    p1 >> h;   //for case when p1 == p2

    double a = p2.x - p1.x;
    double b = p2.y - p1.y;
    double c = p2.z - p1.z;
    double d = -(p2.x - p1.x) * m.x - (p2.y - p1.y) * m.y - (p2.z - p1.z) * m.z;
    
    double lambda;
    if(p1 == p2)
         return h;
    else
        lambda = (-p1.x * a - p1.y * b - p1.z * c - d) 
            / (pow(p2.x - p1.x, 2) + pow(p2.y - p1.y, 2) + pow(p2.z - p1.z, 2));

    h.x = p1.x + (p2.x - p1.x) * lambda;
    h.y = p1.y + (p2.y - p1.y) * lambda;
    h.z = p1.z + (p2.z - p1.z) * lambda;

    return h;
}

void defOrt(vector<point> vert, vector<point> median, vector<point> norm, vector<point>& ort)
{
    ort.clear();

    point h;

    for(int i = 0; i < vert.size(); i++)
    {
        h = defH(vert[i], norm[i], vert[i] + median[i]);
        h = h - vert[i];
        ort.push_back(h);
    }
}

void defTang(vector<point> vert, vector<point> median, vector<point> ort, vector<point>& tang)
{
    tang.clear();
    for(int i = 0; i < vert.size(); i++)
        tang.push_back(median[i] - ort[i]);
}

void initAlgo(int divNum/*, point cent*/, int RADIUS, vector<point>& vert, vector<vector<int>>& vertRel, 
    vector<point>& ort, vector<point>& tang){
    
    initSphere(divNum, RADIUS, vert, vertRel);

    vector<point> norm;
    vector<point> median;

    defMedians(vert, vertRel, median);
    defNorms(RADIUS, vert, vertRel, norm);
    sortRel(vert, vertRel);
    defNorms(RADIUS, vert, vertRel, norm);
    reSort(vert, norm, vertRel);

    defOrt(vert, median, norm, ort);
    defTang(vert, median, ort, tang);
}
