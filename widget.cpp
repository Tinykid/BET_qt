#include <QtGui>
#include <QHBoxLayout>
#include <QSlider>
#include <QPushButton>
#include <QGroupBox>
#include <QMenu>
#include <QMenuBar>
#include <QLabel>
#include <QFileDialog>
#include <QDir>
#include <QMessageBox>

#include "glwidget.h"
#include "widget.h"

Window::Window()
{
    glWidget = new GLWidget;

    lbl = new QLabel(tr("die"));

    QPushButton* btnDef = new QPushButton("Deformation");
    QPushButton* btnDef3 = new QPushButton("Deformation *3");
    QPushButton* btnDefComp1 = new QPushButton("Component #1");
    QPushButton* btnReset = new QPushButton("Reset");
    QPushButton* btnExtr = new QPushButton("Extract");
    QPushButton* btnHideTex = new QPushButton("Hide textures");
    QPushButton* btnUpdateNorm = new QPushButton("UpdateNorm");

    xSlider = createSliderZ();
    ySlider = createSliderXY();
    zSlider = createSliderXY();
/*
    connect(xSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(pickXsurf(int)));
    //connect(glWidget, SIGNAL(xRotationChanged(int)), xSlider, SLOT(setValue(int)));
    connect(ySlider, SIGNAL(valueChanged(int)), glWidget, SLOT(pickYsurf(int)));
    //connect(glWidget, SIGNAL(yRotationChanged(int)), ySlider, SLOT(setValue(int)));
    connect(zSlider, SIGNAL(valueChanged(int)), glWidget, SLOT(pickZsurf(int)));
   // connect(glWidget, SIGNAL(zRotationChanged(int)), zSlider, SLOT(setValue(int)));

    connect(btnDef, SIGNAL(clicked()), glWidget, SLOT(deformD()));
    connect(btnDef3, SIGNAL(clicked()), glWidget, SLOT(deform3()));
    connect(btnDefComp1, SIGNAL(clicked()), glWidget, SLOT(comp1()));
    connect(btnReset, SIGNAL(clicked()), glWidget, SLOT(reset()));
    connect(btnExtr, SIGNAL(clicked()), glWidget, SLOT(extract()));
    connect(btnHideTex, SIGNAL(clicked()), glWidget, SLOT(hideTex()));
    connect(btnUpdateNorm, SIGNAL(clicked()), glWidget, SLOT(updateNorm()));
*/
    QHBoxLayout *mainLayout = new QHBoxLayout;
    mainLayout->addWidget(glWidget);
    mainLayout->addWidget(xSlider);
    mainLayout->addWidget(ySlider);
    mainLayout->addWidget(zSlider);
    mainLayout->addWidget(zSlider);

    createMenu();
    mainLayout->setMenuBar(menuBar);

    QVBoxLayout *cntrLayout = new QVBoxLayout;

    QGroupBox* verticalGroupBox = new QGroupBox(tr("Actions"));

    cntrLayout->addWidget(btnDef);
    cntrLayout->addWidget(btnDef3);
    cntrLayout->addWidget(btnDefComp1);
    cntrLayout->addWidget(btnReset);
    cntrLayout->addWidget(btnExtr);
    cntrLayout->addWidget(btnHideTex);
    cntrLayout->addWidget(btnHideTex);
    cntrLayout->addWidget(btnUpdateNorm);
    verticalGroupBox->setLayout(cntrLayout);

    mainLayout->addWidget(verticalGroupBox);

    setLayout(mainLayout);

    xSlider->setValue(15 * 16);
    ySlider->setValue(345 * 16);
    zSlider->setValue(0 * 16);
    setWindowTitle(tr("Brain Extraction Tools"));

}

void Window::createMenu()
{
    menuBar = new QMenuBar;

    fileMenu = new QMenu(tr("&File"), this);
    exitAction = fileMenu->addAction(tr("E&xit"));

    fileMenu2 = new QMenu(tr("&Help"), this);
    infoAction = fileMenu2->addAction(tr("Info"));

    fileMenu3 = new QMenu(tr("&Edit"), this);
    loadInputAction = fileMenu3->addAction(tr("L&oad Data"));
    loadTemplAction = fileMenu3->addAction(tr("L&oad Templates"));
    resetAction = fileMenu3->addAction(tr("R&eset"));

    menuBar->addMenu(fileMenu);
    menuBar->addMenu(fileMenu2);
    menuBar->addMenu(fileMenu3);

    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));
    connect(resetAction, SIGNAL(triggered()), this, SLOT(slotMenuReset()));
    connect(infoAction, SIGNAL(triggered()), this, SLOT(getInfo()));
    connect(loadInputAction, SIGNAL(triggered()), this, SLOT(setBrainDirectory()));
    connect(loadTemplAction, SIGNAL(triggered()), this, SLOT(setTargetDirectory()));

}

QSlider *Window::createSliderZ()
{
    QSlider *slider = new QSlider(Qt::Vertical);
    slider->setRange(0, /*glWidget->n*/174);
    slider->setSingleStep(16);
    slider->setPageStep(16);
    slider->setTickInterval(10);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}

QSlider *Window::createSliderXY()
{
    QSlider *slider = new QSlider(Qt::Vertical);
    slider->setRange(0, /*glWidget->n*/255);
    slider->setValue(100);
    slider->setSingleStep(16);
    slider->setPageStep(16);
    slider->setTickInterval(10);
    slider->setTickPosition(QSlider::TicksRight);
    return slider;
}

void Window::keyPressEvent(QKeyEvent *e)
{
    if (e->key() == Qt::Key_Escape)
        close();
    else
        QWidget::keyPressEvent(e);
}

void Window::slotMenuHide()
{
    //glWidget->hideTex();
}

void Window::slotMenuReset()
{
    //glWidget->reset();
}

void Window::getInfo()
{
    QString MESSAGE =     "<h4>Link for algorithm description</h4>\n\n"
                          "Algorithm is described on\n"
                          "<a href=\"http://fsl.fmrib.ox.ac.uk/fslcourse/lectures/reg.pdf\">http://fsl.fmrib.ox.ac.uk/fslcourse/lectures/reg.pdf</a>";

    QMessageBox::StandardButton reply;
    reply = QMessageBox::information(this, tr("About application"), MESSAGE);
    /*
    if (reply == QMessageBox::Ok)
        informationLabel->setText(tr("OK"));
    else
        informationLabel->setText(tr("Escape"));
    */
}

void Window::setBrainDirectory()
 {
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    /*if (!native->isChecked())
        options |= QFileDialog::DontUseNativeDialog;*/
    QString directory = QFileDialog::getExistingDirectory(this,
                                tr("QFileDialog::getExistingDirectory()"),
                                /*directoryLabel->text()*/lbl->text(),
                                options);
   // if (!directory.isEmpty())
        //glWidget->dirBrain = directory;
}


void Window::setTargetDirectory()
 {
    QFileDialog::Options options = QFileDialog::DontResolveSymlinks | QFileDialog::ShowDirsOnly;
    /*if (!native->isChecked())
        options |= QFileDialog::DontUseNativeDialog;*/
    QString directory = QFileDialog::getExistingDirectory(this,
                                tr("QFileDialog::getExistingDirectory()"),
                                /*directoryLabel->text()*/lbl->text(),
                                options);
    //if (!directory.isEmpty())
        //glWidget->dirTarget = directory;
}
