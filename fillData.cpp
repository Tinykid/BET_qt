#include "fillData.h"
#include <GL\glut.h>

#include "DrawTriangle.h"

#include <gdcmReader.h>
#include <gdcmImageReader.h>
#include <gdcmImage.h>
#include <gdcmFile.h>
#include <gdcmDicts.h>
#include <gdcmDict.h>
#include <gdcmAttribute.h>
using namespace std;
using namespace gdcm;
#pragma comment(lib,"gdcmMSFF.lib")
#pragma comment(lib,"gdcmDSED.lib")

#define MAX_COLOR 255

void fillData(unsigned int& w, unsigned int& h, unsigned int n){

    // loading data
	char filename[] = "brain4/subject04_t1w_p4001.dcm";
    
	gdcm::ImageReader reader;
	reader.SetFileName(filename);
	if( !reader.Read() )
	{
		cerr << "Could not read: " << filename << endl;
		//return 1;
	}
	File &file = reader.GetFile();
	const Image &image = reader.GetImage();
	unsigned int ndim = image.GetNumberOfDimensions();

	const unsigned int *dims = image.GetDimensions();
	if(w == 0 && h == 0)
	{
		w = dims[0];
		h = dims[1];
        
	}
	else if(w != dims[0] || h != dims[1])
	{
		cerr << "Dimensions don't agree" << endl;
		//return 2;
	}
}

void deleteWhite(int w, int h, int n, unsigned char** volume)
{
    for(int i = 0; i < n; i++)
        for(int j = 0; j < w * h; j++)
            if(volume[i][j] > 190)
                volume[i][j] = 0;
}

bool loadFromFile(int w, int h, const char* fileName, short* imageData)
{
    FILE* fi(fopen(fileName, "rb"));
    if(fi)
    {
        int size = w * h;
        fseek(fi, -(size * 2), SEEK_END);
        fread(imageData, sizeof(short), size, fi);
        fclose(fi);
        
		int max = 0;
		for(int pixNo = 0; pixNo < size; pixNo++)
            if(imageData[pixNo] > max)
                max = imageData[pixNo];

		//Normalizing
		for(int pixNo = 0; pixNo < size; pixNo++)
			imageData[pixNo] = imageData[pixNo] * MAX_COLOR / max;
		
	    return true;
    }
    else
	    return false;
}

void initTextures(int w, int h, int n, unsigned char*** buffers, unsigned char** volume){

    for(int i = 0; i < n; i++) 
	{
        buffers[0][i]=new unsigned char[w * h * 3];

		for(int j = 0; j < w * h; j++)
			buffers[0][i][j * 3] = buffers[0][i][j * 3 + 1] = buffers[0][i][j * 3 + 2] = volume[i][j];
    }

	for(int i = 0; i < w; i++)
	{
		buffers[1][i] = new unsigned char[n * h * 3];
		for(int j = 0; j < n; j++)
			for(int k = 0; k < h; k++)
				buffers[1][i][(k + j * h) * 3] = buffers[1][i][(k + j * h) * 3 + 1] = buffers[1][i][(k + j * h) * 3 + 2] = buffers[0][j][(i + w * k) * 3];
	}

	for(int i = 0; i < h; i++)
	{
		buffers[2][i] = new unsigned char[n * w * 3];
		for(int j = 0; j < n; j++)
			for(int k = 0; k < w; k++)
				buffers[2][i][(k + j * h) * 3] = buffers[2][i][(k + j * h) * 3 + 1] = buffers[2][i][(k + j * h) * 3 + 2] = buffers[0][j][(k + w * i) * 3];
	}
}

void subExec(int w, int h, int n, unsigned char** volume, bool* mask){

    int nnn = 0;
    
    for(long k = 0; k < n * w * h; k++)
        mask[k] = 1;

    for(int i = 0; i < w * h; i++)
    {
        while(volume[nnn][i] != MAX_COLOR && nnn < n - 1){
            mask[nnn * w * h + i] = 0;
            nnn++;
        }
        nnn = 0;
    }

    nnn = n - 1;
    for(int i = 0; i < w * h; i++)
    {
        while(volume[nnn][i] != MAX_COLOR && nnn > 1){
            mask[nnn * w * h + i] = 0;
            nnn--;
        }
        nnn = n - 1;
    }
}

void subInit(int w, int h, int n, unsigned int** tex, unsigned char*** buffers)
{
    glEnable(GL_TEXTURE_2D);
	
    glGenTextures(n, tex[0]);
    glGenTextures(w, tex[1]);
    glGenTextures(h, tex[2]);

	for(int i = 0; i < n; i++)
	{
		glBindTexture(GL_TEXTURE_2D, tex[0][i]);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, buffers[0][i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	}

	for(int i = 0; i < w; i++)
	{
		glBindTexture(GL_TEXTURE_2D, tex[1][i]);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, h, n, 0, GL_RGB, GL_UNSIGNED_BYTE, buffers[1][i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	}

	for(int i = 0; i < h; i++)
	{
		glBindTexture(GL_TEXTURE_2D, tex[2][i]);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, n, 0, GL_RGB, GL_UNSIGNED_BYTE, buffers[2][i]);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); 
	}

	glEnable(GL_DEPTH_TEST);
	glMatrixMode (GL_MODELVIEW);
}


void extraction(int w, int h, int n, std::vector<point> vert, std::vector<std::vector<int>> vertRel,
    unsigned int** tex, unsigned char*** buffers, unsigned char** volume)
{
    vector<point> buff;
    vector<point> surf;

    for(int i = 0; i < vert.size(); i++)
    {
        for(int j = 1; j < vertRel[i].size(); j++)
        {
            drawTriangl(vert[i], vert[vertRel[i][j - 1]], vert[vertRel[i][j]], buff);
            for(int k = 0; k < buff.size(); k++)
                surf.push_back(buff[k]);
        }
        drawTriangl(vert[i], vert[vertRel[i][vertRel[i].size() - 1]], vert[vertRel[i][0]], buff);
        for(int k = 0; k < buff.size(); k++)
        {
            surf.push_back(buff[k]);
        }
    }

    for(int i = 0; i < surf.size(); i++)
        volume[int(surf[i].z + n / 2)][int(surf[i].y + h / 2) * w + int(surf[i].x + w / 2)] = MAX_COLOR;
    /*
    bool* mask = new bool [n * w * h];

    exec3(mask);

    for(int i = 0; i < n; i++)
        for(int j = 0; j < w * h; j++)
            if(mask[i * w * h + j] == 0)
                volume[i][j] = 0;
    */
    initTextures(w, h, n, buffers, volume);
    subInit(w, h, n, tex, buffers);
}


//load bmp file into int array
int * loadBMP24(const char * fileName, int w, int h)
{
	FILE * pFile = fopen(fileName, "rb");
 	for(int i = 0; i < 54; i++)
		getc(pFile);
     
    int *v = new int[w * h * 3];

    for (int i = h - 1; i >= 0; i--) {
		for (int j = 0; j < w; j++) 
		{
            
			v[(j * w + i) * 3] = getc(pFile);
            v[(j * w + i) * 3 + 1] = getc(pFile);
            v[(j * w + i) * 3 + 2] = getc(pFile);	
            
        }

    };
 
    fclose(pFile);
    return v;
}

