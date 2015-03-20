#include <QtGui>
#include <QtOpenGL>

#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>

#include "glwidget.h"
#include "Algo.h"
#include "fillData.h"
#include "Deformation.h"

#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

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

const int vertNum = defVertNum(3);
std::vector<std::vector<int>> vertRel(vertNum, std::vector<int>());  //Each vertex has 5 or 6 neigbours

point ppp[7];
point nnn;

#define MAX_COLOR 255

 GLWidget::GLWidget(QWidget *parent)
     : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
 {
     w = 256;
     h = 256;
     n = 175;
     RADIUS = 40;
     currTexture[0] = 80;
     currTexture[1] = 120;
     currTexture[2] = 120;
     divNum = 3;
     flag = 0;

     volume = new unsigned char*[n];

     xRot = 0;
     yRot = 0;
     zRot = 0;
     dirName = "brain4/subject04_t1w_p4001.dcm";

     qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
     qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);

     initAlgo(3, 40, vert, vertRel, ort, tang);

     fillData(w, h, n);

     tex[0] = new unsigned int[n];
     tex[1] = new unsigned int[w];
     tex[2] = new unsigned int[h];
     buffers[0] = new unsigned char*[n];
     buffers[1] = new unsigned char*[w];
     buffers[2] = new unsigned char*[h];

     reloadData();
/*
     initVol();

     deleteWhite(w, h, n, volume);

     initTextures(w, h, n, buffers, volume);
*/
 }

 GLWidget::~GLWidget()
 {
 }

 QSize GLWidget::minimumSizeHint() const
 {
     return QSize(50, 50);
 }

 QSize GLWidget::sizeHint() const
 {
     return QSize(400, 400);
 }

 static void qNormalizeAngle(int &angle)
 {
     while (angle < 0)
         angle += 360 * 16;
     while (angle > 360 * 16)
         angle -= 360 * 16;
 }

 void GLWidget::setXRotation(int angle)
 {
     qNormalizeAngle(angle);
     if (angle != xRot) {
         xRot = angle;
         emit xRotationChanged(angle);
         updateGL();
     }
 }

 void GLWidget::setYRotation(int angle)
 {
     qNormalizeAngle(angle);
     if (angle != yRot) {
         yRot = angle;
         emit yRotationChanged(angle);
         updateGL();
     }
 }

 void GLWidget::setZRotation(int angle)
 {
     qNormalizeAngle(angle);
     if (angle != zRot) {
         zRot = angle;
         emit zRotationChanged(angle);
         updateGL();
     }
 }

 void GLWidget::initVol()
 {
     GLushort *buffer = new GLushort[w * h];

     //dirName = "brain4/subject04_t1w_p4001.dcm";
     loadFromFile(w, h, dirName, buffer);

     for(int i = 0; i < 10; i++)
     {
         //sprintf(filename, "brain4/subject04_t1w_p400%i.dcm", i + 1);
         dirName[25] = '0' + i;

         loadFromFile(w, h, dirName, buffer);

         volume[i] = new unsigned char[w * h];
         for(int j = 0; j < w * h; j++)
             volume[i][j] = buffer[j];
     }

     for(int i = 10; i < 100; i++)
     {
         //sprintf(filename, "brain4/subject04_t1w_p400%i.dcm", i + 1);

         dirName = "brain4/subject04_t1w_p40010.dcm";
         dirName[25] = '0' + int(i / 10) % 10;
         dirName[26] = '0' + i % 10;

         loadFromFile(w, h, dirName, buffer);

         volume[i] = new unsigned char[w * h];
         for(int j = 0; j < w * h; j++)
             volume[i][j] = buffer[j];
     }

     for(int i = 100; i < n; i++)
     {
         //sprintf(filename, "brain4/subject04_t1w_p40%i.dcm", i + 1);

        dirName = "brain4/subject04_t1w_p40110.dcm";
        dirName[26] = '0' + i % 10;
        dirName[25] = '0' + int(i / 10) % 10;

         loadFromFile(w, h, dirName, buffer);
         volume[i] = new unsigned char[w * h];
         for(int j = 0; j < w * h; j++)
             volume[i][j] = buffer[j];
     }
 }

 void GLWidget::reloadData()
 {
     initVol();
     deleteWhite(w, h, n, volume);
     initTextures(w, h, n, buffers, volume);
 }

 void GLWidget::deformD()
 {
     deformationD(40, w, h, n, vert, vertRel, ort, tang, volume);
     deformationD(40, w, h, n, vert, vertRel, ort, tang, volume);
     deformationD(40, w, h, n, vert, vertRel, ort, tang, volume);
     updateGL();
 }

 void GLWidget::deform3()
 {
     deformation(40, w, h, n, vert, ort, tang, volume);
     deformation(40, w, h, n, vert, ort, tang, volume);
     deformation(40, w, h, n, vert, ort, tang, volume);
     updateGL();
 }

 void GLWidget::comp1()
 {
     ppp[0].z -= 5;
     nnn = normalize(defNormal(ppp[0], ppp[6], ppp[5], ppp[4], ppp[3], ppp[2], ppp[1]), 50);
     updateGL();
 }

 void GLWidget::reset()
 {
     initSphere(divNum, RADIUS, vert, vertRel);
     updateGL();
 }

 void GLWidget::extract()
 {
     extraction(w, h, n, vert, vertRel, tex, buffers, volume);
     updateGL();
 }

 void GLWidget::hideTex()
 {
     flag = (flag + 1) % 2;
     updateGL();
 }

 void GLWidget::loadData()
 {
     flag = (flag + 1) % 2;
     updateGL();
 }

 void GLWidget::updateNorm()
 {
     normUpdate(vert, vertRel);

     //Check norms

     updateGL();
 }
/*
 void GLWidget::setDir(QString dir)
 {
     this->dirName = dir;
 }

 void GLWidget::setDirTempl(QString dir)
 {
     this->dirTempl = dir;
 }
*/

 void GLWidget::pickXsurf(int val)
 {
     currTexture[0] = val;
     updateGL();
 }

 void GLWidget::pickYsurf(int val)
 {
     currTexture[1] = val;
     updateGL();
 }

 void GLWidget::pickZsurf(int val)
 {
     currTexture[2] = val;
     updateGL();
 }

 void GLWidget::subInit()
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

 void GLWidget::initializeGL()
 {
     qglClearColor(qtPurple.dark());

     glOrtho(-200, 200, -200, 200, -2500, 2500);
     subInit();

 }

 void GLWidget::paintGL()
 {
     glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
     glLoadIdentity();
     glTranslatef(0.0, 0.0, -10.0);
     glRotatef(xRot / 16.0, 1.0, 0.0, 0.0);
     glRotatef(yRot / 16.0, 0.0, 1.0, 0.0);
     glRotatef(zRot / 16.0, 0.0, 0.0, 1.0);

     glPointSize(2.5);
     glBegin(GL_POINTS);
         for(int i = 0; i < vert.size(); i++)
             glVertex3d(vert[i].x, vert[i].y, vert[i].z);
     glEnd();

     glColor3d(1.0, 1.0, 1.0);
     glBegin(GL_LINES);
         for(int i = 0; i < vert.size(); i++)
         {
             glVertex3d(vert[i].x, vert[i].y, vert[i].z);
             glVertex3d(vert[i].x - 10 * ort[i].x / dist(ort[i]), vert[i].y - 10 * ort[i].y / dist(ort[i]),
                 vert[i].z - 10 * ort[i].z / dist(ort[i]));
         }
     glEnd();

/*
     glColor3d(1.0, 0.0, 0.0);
     glPointSize(5);
     glBegin(GL_POINTS);
         for(int i = 0; i < 7; i++)
             glVertex3d(ppp[i].x, ppp[i].y, ppp[i].z);
         glVertex3d(ppp[0].x + nnn.x, ppp[0].y + nnn.y, ppp[0].z + nnn.z);
     glEnd();

     glColor3d(0.0, 1.0, 0.0);
     glBegin(GL_LINES);
         for(int i = 1; i < 7; i++)
         {
             glVertex3d(ppp[0].x, ppp[0].y, ppp[0].z);
             glVertex3d(ppp[i].x, ppp[i].y, ppp[i].z);
         }

         glVertex3d(ppp[0].x, ppp[0].y, ppp[0].z);
         glVertex3d(ppp[0].x + nnn.x, ppp[0].y + nnn.y, ppp[0].z + nnn.z);

     glEnd();
*/
     //borders of textures
     double koef = 1;
     int ww = w / 2;
     int hh = h / 2;
     int nn = n * koef / 2;
     //bool flag = 1;

     glLineWidth(3.0);
     if(flag)
     {
     glBegin(GL_LINE_STRIP);
         glColor3d(1.0, 0.0, 0.0);
         glVertex3f(-ww, -hh, currTexture[0] * koef - nn);
         glVertex3f(ww, -hh, currTexture[0] * koef - nn);
         glVertex3f(ww, hh, currTexture[0] * koef - nn);
         glVertex3f(-ww, hh, currTexture[0] * koef - nn);
         glVertex3f(-ww, -hh, currTexture[0] * koef - nn);
     glEnd();
     glBegin(GL_LINE_STRIP);
         glColor3d(0.0, 1.0, 0.0);
         glVertex3f(int(currTexture[1] - ww), -hh, -nn);
         glVertex3f(int(currTexture[1] - ww), hh, -nn);
         glVertex3f(int(currTexture[1] - ww), hh, nn);
         glVertex3f(int(currTexture[1] - ww), -hh, nn);
         glVertex3f(int(currTexture[1] - ww), -hh, -nn);
     glEnd();
     glBegin(GL_LINE_STRIP);
         glColor3d(0.0, 0.0, 1.0);
         glVertex3f(-ww, int(currTexture[2] - hh), -nn);
         glVertex3f(ww, int(currTexture[2] - hh), -nn);
         glVertex3f(ww, int(currTexture[2] - hh), nn);
         glVertex3f(-ww, int(currTexture[2] - hh), nn);
         glVertex3f(-ww, int(currTexture[2] - hh), -nn);
     glEnd();

     glColor3d(1.0, 1.0, 1.0);
     glBindTexture(GL_TEXTURE_2D, tex[0][currTexture[0]]);
     glBegin(GL_QUADS);
         glTexCoord2f(0, 0);
         glVertex3f(-ww, -hh, currTexture[0] * koef - nn);
         glTexCoord2f(1, 0);
         glVertex3f(ww, -hh, currTexture[0] * koef - nn);
         glTexCoord2f(1, 1);
         glVertex3f(ww, hh, currTexture[0] * koef - nn);
         glTexCoord2f(0, 1);
         glVertex3f(-ww, hh, currTexture[0] * koef - nn);
     glEnd();
     glBindTexture(GL_TEXTURE_2D, tex[1][currTexture[1]]);
     glBegin(GL_QUADS);
         glTexCoord2f(0, 0);
         glVertex3f(int(currTexture[1]) - ww, -hh, -nn);
         glTexCoord2f(1, 0);
         glVertex3f(int(currTexture[1]) - ww, hh, -nn);
         glTexCoord2f(1, 1);
         glVertex3f(int(currTexture[1]) - ww, hh, nn);
         glTexCoord2f(0, 1);
         glVertex3f(int(currTexture[1]) - ww, -hh, nn);
     glEnd();
     glBindTexture(GL_TEXTURE_2D, tex[2][currTexture[2]]);
     glBegin(GL_QUADS);
         glTexCoord2f(0, 0);
         glVertex3f(-ww, int(currTexture[2] - hh), -nn);
         glTexCoord2f(1, 0);
         glVertex3f(ww, int(currTexture[2] - hh), -nn);
         glTexCoord2f(1, 1);
         glVertex3f(ww, int(currTexture[2] - hh), nn);
         glTexCoord2f(0, 1);
         glVertex3f(-ww, int(currTexture[2] - hh), nn);
     glEnd();
     }

     glBindTexture(GL_TEXTURE_2D, 0);

     glFlush();
 }

 void GLWidget::resizeGL(int width, int height)
 {
     int side = qMin(width, height);
     glViewport((width - side) / 2, (height - side) / 2, side, side);

     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();
 #ifdef QT_OPENGL_ES_1
     glOrthof(-200, +200, -200, +200, -200, 200);
 #else
     glOrtho(-200, +200, -200, +200, -200, 200);
 #endif
     glMatrixMode(GL_MODELVIEW);
 }

 void GLWidget::mousePressEvent(QMouseEvent *event)
 {
     lastPos = event->pos();
 }

 void GLWidget::mouseMoveEvent(QMouseEvent *event)
 {
     int dx = event->x() - lastPos.x();
     int dy = event->y() - lastPos.y();

     if (event->buttons() & Qt::LeftButton) {
         setXRotation(xRot + 8 * dy);
         setYRotation(yRot + 8 * dx);
     } else if (event->buttons() & Qt::RightButton) {
         setXRotation(xRot + 8 * dy);
         setZRotation(zRot + 8 * dx);
     }
     lastPos = event->pos();
 }

 void GLWidget::keyPressEvent(QKeyEvent *event)
 {
     /*
     switch (event->key())
     {
     case Qt::Key_Space:
         //flag = 0;
         setXRotation(xRot + 8 * 5);
         repaint();
         updateGL();
         break;
     default:
         break;
     }
     */
     if(event->key() == 'h')
     {
         setXRotation(xRot + 8 * 5);
         repaint();
         updateGL();

     }
 }

 void GLWidget::loadFromFile(int w, int h, QString fileName, GLushort* imageData)
 {
     FILE* fi(fopen(fileName.toStdString().c_str(), "rb"));
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

         //return true;
     }
     //else
         //return false;
 }

