#ifndef FILLDATA_H
#define FILLDATA_H

#include <vector>
#include "VectOperators.h"

//void fillData(unsigned int& w, unsigned int& h, unsigned int n);

void deleteWhite(int w, int h, int n, unsigned char** volume);

//bool loadFromFile(int w, int h, const char* fileName, short* imageData);

void initTextures(int w, int h, int n, unsigned char*** buffers, unsigned char** volume);

//void subInit(int w, int h, int n, unsigned int** tex, unsigned char*** buffers);

void extraction(int w, int h, int n, std::vector<point> vert, std::vector<std::vector<int> > vertRel,
    unsigned int** tex, unsigned char*** buffers, unsigned char** volume);

int * loadBMP24(const char * fileName, int w, int h);

#endif
