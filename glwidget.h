#ifndef GLWIDGET_H
#define GLWIDGET_H

#include <QGLWidget>
#include <QPoint>
#include <QColor>
#include <QKeyEvent>

 class GLWidget : public QGLWidget
 {
     Q_OBJECT

 public:
     GLWidget(QWidget *parent = 0);
     ~GLWidget();

     QSize minimumSizeHint() const;
     QSize sizeHint() const;

 protected:
     void initializeGL();
     void paintGL();
     void resizeGL(int width, int height);

 private:

     QPoint lastPos;
     QColor qtGreen;
     QColor qtPurple;
 };

 #endif
