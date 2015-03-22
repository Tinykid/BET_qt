#include <QtGui>
#include <QtOpenGL>
#include "glwidget.h"
/*
#include <math.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>

#include "fillData.h"
#include "Deformation.h"
*/
#ifndef GL_MULTISAMPLE
#define GL_MULTISAMPLE  0x809D
#endif

#define MAX_COLOR 255

 GLWidget::GLWidget(QWidget *parent)
     : QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
 {

     qtGreen = QColor::fromCmykF(0.40, 0.0, 1.0, 0.0);
     qtPurple = QColor::fromCmykF(0.39, 0.39, 0.0, 0.0);
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

 void GLWidget::initializeGL()
  {
      qglClearColor(qtPurple.dark());

      glOrtho(-200, 200, -200, 200, -2500, 2500);


  }

 void GLWidget::paintGL()
 {

     glColor3d(1.0, 1.0, 1.0);

     glBegin(GL_TRIANGLES);
         glVertex3d(100, 100, 0);
         glVertex3d(100, 0, 0);
         glVertex3d(0, 100, 0);
     glEnd();

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



