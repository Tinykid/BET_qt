#include "Deformation.h"

using namespace std;

std::vector<point> median;
std::vector<point> norm;
//std::vector<point> ort;
//std::vector<point> tang;

int minI(point p1, point p2, int w, int h, int n, unsigned char** volume)
{
    p1.x += w / 2;
    p1.y += h / 2;
    p1.z += n / 2;

    p2.x += w / 2;
    p2.y += h / 2;
    p2.z += n / 2;

    int min;
    min = volume[int(p1.z)][int(p1.y) * w + int(p1.x)];

    int stepNum = 2 * int(dist(p2 - p1));
    point step = (p2 - p1) / stepNum;
    
    point curr;

    for(int i = 1; i < stepNum - 5; i++)
    {
        curr = p1 + i * step;
        if(3 < curr.x && curr.x < w - 3 && 3 < curr.y && curr.y < h- 3 
            && 3 < curr.z && curr.z < n)
            if(volume[int(curr.z)][int(curr.y) * w + int(curr.x)] < min)
                min = volume[int(curr.z)][int(curr.y) * w + int(curr.x)];
    }

    return min;
}

int maxI(point p1, point p2, int w, int h, int n, unsigned char** volume)
{
    p1.x += w / 2;
    p1.y += h / 2;
    p1.z += n / 2;

    p2.x += w / 2;
    p2.y += h / 2;
    p2.z += n / 2;

    int max;
    max = volume[int(p1.z)][int(p1.y) * w + int(p1.x)];

    int stepNum = 2 * int(dist(p2 - p1));
    point step = (p2 - p1) / stepNum;
    
    point curr;

    for(int i = 1; i < stepNum - 5; i++)
    {
        curr = p1 + i * step;
        if(3 < curr.x && curr.x < w - 3 && 3 < curr.y && curr.y < h- 3 
            && 3 < curr.z && curr.z < n)
            if(volume[int(curr.z)][int(curr.y) * w + int(curr.x)] > max)
                max = volume[int(curr.z)][int(curr.y) * w + int(curr.x)];
    }

    return max;
}

void deformation(int RADIUS, int w, int h, int n, vector<point>& vert,  vector<point>& ort,  vector<point>& tang, unsigned char** volume)
{
    int d2 = 20;
    point next;

    for(int i = 0; i < vert.size(); i++)
    {
        
        next = vert[i] + ort[i];
        next = vert[i] + (d2 / dist(vert[i], next)) * ort[i];  //after this operation dist(vert[i], next) = 20 and I need pixels beetwen vert[i]
                                                               //and next
        int i_min;
        int i_max;
        
        if(-double(w) / 2 + 3 < next.x && next.x < w / 2 - 3 && -double(h) / 2 + 3 < next.y && next.y < h / 2 - 3 && -double(n) / 2 + 3 < next.x && next.x < n / 2 - 3)
        {
            i_min = minI(vert[i], next, w, h, n, volume);
            i_max = maxI(vert[i], next, w, h, n, volume);
        

          if(i_min < 16)
          {
            next = vert[i] + normalize(ort[i], RADIUS) / 100;
            if(-double(w) / 2 + 3 < next.x && next.x < double(w) / 2 - 3 && -double(h) / 2 + 3 < next.y && next.y < double(h) / 2 - 3 
                && -double(n) / 2 + 3 < next.z && next.z < double(n) / 2 - 3)
                vert[i] = next;
          }
          else
          {
            next = vert[i] - normalize(ort[i], RADIUS) / 100;
            if(-double(w) / 2 + 3 < next.x && next.x < double(w) / 2 - 3 && -double(h) / 2 + 3 < next.y && next.y < double(h) / 2 - 3 
                && -double(n) / 2 + 3 < next.z && next.z < double(n) / 2 - 3)
                vert[i] = next;
          }
        }
    }

    /*
    defMedians();
    defNorms();
    defOrt();
    defTang();
    */
    /*/1st component
    for(int i = 0; i < vert.size(); i++)
    {
        vert[i] = vert[i] + 0.5 * tang[i];
    }*/

    /*
    //2nd component
    for(int i = 0; i < vert.size(); i++)
    {
        double minDist;
        point h = vert[i] + ort[i];
        minDist = dist(vert[vertRel[i][0]], h);

        for(int j = 1; j < vertRel[i].size(); j++)
            if(minDist < dist(vert[vertRel[i][j]], h))
                minDist = dist(vert[vertRel[i][j]], h);

        double curv;
        curv = dist(vert[i], h) / minDist;
           
        if(curv > 0.2)
            vert[i] = vert[i] + 0.2 * ort[i] / curv;
    }
    */
    /*
    defMedians();
    defNorms();
    defOrt();
    defTang();
    */
}

void deformationD(int RADIUS, int w, int h, int n, vector<point>& vert, vector<vector<int> > vertRel,  vector<point>& ort,  vector<point>& tang, unsigned char** volume)
{
    int d2 = 20;
    point next;

    for(int i = 0; i < vert.size(); i++)
    {

        next = vert[i] + ort[i];
        next = vert[i] + (d2 / dist(vert[i], next)) * ort[i];  //after this operation dist(vert[i], next) = 20 and I need pixels beetwen vert[i]
                                                               //and next
        int i_min;
        int i_max;

        if(-double(w) / 2 + 3 < next.x && next.x < w / 2 - 3 && -double(h) / 2 + 3 < next.y && next.y < h / 2 - 3 && -double(n) / 2 + 3 < next.x && next.x < n / 2 - 3)
        {
            i_min = minI(vert[i], next, w, h, n, volume);
            i_max = maxI(vert[i], next, w, h, n, volume);


          if(i_min < 16)
          {
            next = vert[i] + normalize(ort[i], RADIUS) / 100;
            if(-double(w) / 2 + 3 < next.x && next.x < double(w) / 2 - 3 && -double(h) / 2 + 3 < next.y && next.y < double(h) / 2 - 3
                && -double(n) / 2 + 3 < next.z && next.z < double(n) / 2 - 3)
                vert[i] = next;
          }

          else
          {
            next = vert[i] - normalize(ort[i], RADIUS) / 100;
            if(-double(w) / 2 + 3 < next.x && next.x < double(w) / 2 - 3 && -double(h) / 2 + 3 < next.y && next.y < double(h) / 2 - 3
                && -double(n) / 2 + 3 < next.z && next.z < double(n) / 2 - 3)
                vert[i] = next;
          }

        }
    }

    defMedians(vert, vertRel, median);
    defNorms(40, vert, vertRel, norm);
    defOrt(vert, median, norm, ort);
    defTang(vert, median, ort, tang);
}

void deformationComp1(int RADIUS, int w, int h, int n, vector<point>& vert,  vector<point>& ort,  vector<point>& tang, unsigned char** volume)
{
    //1st component
    for(int i = 0; i < vert.size(); i++)
    {
        vert[i] = vert[i] + 0.5 * tang[i];
    }
}

void normUpdate(vector<point>& vert,  vector<vector<int> > vertRel)
{
    defNorms(40,  vert, vertRel, norm);
}
