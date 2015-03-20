#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QPoint>
#include <QColor>
#include <QKeyEvent>

#include <vector>
#include "VectOperators.h"

 class GLWidget : public QGLWidget
 {
     Q_OBJECT

 public:
     GLWidget(QWidget *parent = 0);
     ~GLWidget();

     QSize minimumSizeHint() const;
     QSize sizeHint() const;
     void loadFromFile(int w, int h, QString filename, GLushort* buffer);
     void initVol();
     void reloadData();
     void subInit();

     QString dirName;
     QString dirTempl;

     std::vector<point> vert;
     std::vector<point> ort;
     std::vector<point> tang;
     unsigned char **buffers[3];
     unsigned int *tex[3];
     unsigned int w;
     unsigned int h;
     unsigned int n;
     unsigned char** volume;
     unsigned int currTexture[3];
     bool flag;
     int divNum;
     int RADIUS;

 public slots:
     void setXRotation(int angle);
     void setYRotation(int angle);
     void setZRotation(int angle);
     void deformD();
     void deform3();
     void comp1();
     void reset();
     void extract();
     void hideTex();
     void loadData();
     void updateNorm();
     //void setDir(QString dir);
     //void setDirTempl(QString dir);
     void pickXsurf(int val);
     void pickYsurf(int val);
     void pickZsurf(int val);

 signals:
     void xRotationChanged(int angle);
     void yRotationChanged(int angle);
     void zRotationChanged(int angle);

 protected:
     void initializeGL();
     void paintGL();
     void resizeGL(int width, int height);
     void mousePressEvent(QMouseEvent *event);
     void mouseMoveEvent(QMouseEvent *event);
     void keyPressEvent(QKeyEvent *event);

 private:
     int xRot;
     int yRot;
     int zRot;

     QPoint lastPos;
     QColor qtGreen;
     QColor qtPurple;
 };

 #endif
