#ifndef WINDOW_H
#define WINDOW_H

#include <QWidget>
#include <QDir>


class QMenu;
class QMenuBar;
class QLabel;
class QSlider;
class GLWidget;
class QDir;

class Window : public QWidget
{
    Q_OBJECT

public:
    Window();
    QLabel* lbl;

public slots:
    void slotMenuHide();
    void slotMenuReset();
    void getInfo();
    void setBrainDirectory();
    void setTargetDirectory();

protected:
    void keyPressEvent(QKeyEvent *event);

private:
    QSlider *createSliderZ();
    QSlider *createSliderXY();
    void createMenu();

    QMenuBar *menuBar;
    GLWidget *glWidget;
    QSlider *xSlider;
    QSlider *ySlider;
    QSlider *zSlider;

    QMenu *fileMenu;
    QMenu *fileMenu2;
    QMenu *fileMenu3;
    QAction *exitAction;
    QAction *resetAction;
    QAction *infoAction;
    QAction *loadInputAction;
    QAction *loadTemplAction;

    QDir currentDir;
};

#endif
