#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include "vectorform.h"
#include "colorform.h"
#include "sliderlabel.h"
#include "scatteringmaterialgui.h"
using namespace std;
using namespace CGLA;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->clearColor->init("Clear color: ", ui->translucentMaterials->getClearColor());
    connect(ui->clearColor,SIGNAL(vectorChanged(CGLA::Vec4f&)),ui->translucentMaterials,SLOT(setClearColor(CGLA::Vec4f&)));
    Vec3f userpos = Vec3f(-0.01684, -0.2174, 0.1101);// buddha
    //Vec3f userpos = Vec3f(0.016, 1.03, 0.054);// buddha
    //Vec3f userpos = Vec3f(-0.1,-1.2,0.8); //bunny
    //Vec3f userpos = Vec3f(-1.08199,-0.701759,0.8); //dragon cloaseup
    //Vec3f userpos = Vec3f(0,-2.2,0); //dragon
    //Vec3f userpos = Vec3f(0,1,0);
    ui->userPosition->init("User position: ", userpos);
    ui->translucentMaterials->setUserPosition(userpos);
    connect(ui->userPosition,SIGNAL(vectorChanged(CGLA::Vec3f&)),ui->translucentMaterials,SLOT(setUserPosition(CGLA::Vec3f&)));
    connect(ui->translucentMaterials,SIGNAL(userPositionChanged(CGLA::Vec3f&)),ui->userPosition,SLOT(setValue(CGLA::Vec3f&)));

    //Vec3f userdir = Vec3f(0,0.918,-0.397);
    Vec3f userdir = Vec3f(0,1,0);
    //Vec3f userdir = Vec3f(0.51831,0.69886,-0.492898); //dragon closeup

    ui->userDirection->init("User direction: ", userdir);
    ui->translucentMaterials->setUserDirection(userdir);
    connect(ui->userDirection, SIGNAL(vectorChanged(CGLA::Vec3f&)),ui->translucentMaterials,SLOT(setUserDirection(CGLA::Vec3f&)));
    connect(ui->translucentMaterials,SIGNAL(userDirectionChanged(CGLA::Vec3f&)),ui->userDirection,SLOT(setValue(CGLA::Vec3f&)));

    ui->translucentMaterials->setLightIntensity(5.0f);
    ui->intensity->setValue(5);

    connect(ui->showgrid, SIGNAL(toggled(bool)), ui->translucentMaterials, SLOT(setGridVisible(bool)));
    ui->showgrid->setChecked(false);

    connect(ui->showaxes, SIGNAL(toggled(bool)), ui->translucentMaterials, SLOT(setAxesVisible(bool)));
    ui->showaxes->setChecked(false);

    connect(ui->translucentMaterials, SIGNAL(timeMeasurement(int)), this, SLOT(timeMeasurement(int)));
    ui->fpslabel->setTextInteractionFlags(Qt::TextSelectableByMouse);

    GLGraphics::ThreeDObject * bunny = ui->translucentMaterials->getDefaultObject();
    ui->objTest->setObject(bunny);
    connect(ui->objTest,SIGNAL(modelChanged(QString &)), this, SLOT(modelChanged(QString &)));

    ui->radius->init("Radius: ", ui->translucentMaterials->getParameters()->circleradius, 1.0, 50.0, false);
    connect(ui->radius, SIGNAL(valueChanged(float)), this, SLOT(radiusChanged(float)));


    ui->samples->init("Samples (per all lights): ",ui->translucentMaterials->getParameters()->samples,1,500, true);
    connect(ui->samples, SIGNAL(valueChanged(float)), this, SLOT(samplesChanged(float)));

    ui->maxsamples->init("Max samples: ",ui->translucentMaterials->getParameters()->maxsamples,50000,1000000, true);
    connect(ui->maxsamples, SIGNAL(valueChanged(float)), this, SLOT(maxsamplesChanged(float)));


    ui->epsilon_combination->init("Comb. offset: ", ui->translucentMaterials->getParameters()->epsilon_combination, 0.0, 0.05, false);
    connect(ui->epsilon_combination,SIGNAL(valueChanged(float)), this, SLOT(epsilonCombinationChanged(float)));

    ui->shadow_bias->init("Shadow bias: ", ui->translucentMaterials->getParameters()->shadow_bias, 0.0, 0.03, false);
    connect(ui->shadow_bias, SIGNAL(valueChanged(float)), this, SLOT(shadowBiasChanged(float)));

    ui->lod->init("Lod: ", ui->translucentMaterials->getParameters()->LOD, 0, 3, true);
    connect(ui->lod, SIGNAL(valueChanged(float)), this, SLOT(LODChanged(float)));

    ui->a->init("A: ", ui->translucentMaterials->getParameters()->a, 0, 10, false);
    connect(ui->a, SIGNAL(valueChanged(float)), this, SLOT(achanged(float)));

    ui->b->init("B: ", ui->translucentMaterials->getParameters()->b, 0, 50, false);
    connect(ui->b, SIGNAL(valueChanged(float)), this, SLOT(bchanged(float)));

    ui->gamma->init("Gamma: ", ui->translucentMaterials->getParameters()->gamma, 0.1, 3.5, false);
    connect(ui->gamma, SIGNAL(valueChanged(float)), this, SLOT(gammaChanged(float)));

    ui->materialTest->setObject(bunny);

    QButtonGroup * group = new QButtonGroup();
    group->addButton(ui->jensenbutton,0);
    group->addButton(ui->directionalbutton,1);

    ui->environment->setChecked(ui->translucentMaterials->getParameters()->environment);
}

MainWindow::~MainWindow()
{
    delete ui;
}

int main(int argc, char *argv[])
{

    qDebug() << argv[0] << " - " << argv[1] << endl;
    QApplication a( argc, argv );

    MainWindow w;
    w.setFocusPolicy(Qt::ClickFocus);
    w.setWindowTitle("Translucent Material Demo v0.1");
    w.show();

    return a.exec();
}

void MainWindow::on_actionSave_Image_triggered()
{
    QString fileName = QFileDialog::getSaveFileName(this, tr("Save Image"), QString(),
                tr("PNG files (*.png);;JPEG Files (*.jpg *.jpeg)"));

    if (!fileName.isEmpty()) {

        QImage * image = ui->translucentMaterials->takeScreenshot();
        image->save(fileName);

    }

}

void MainWindow::on_actionQuit_triggered()
{
    qApp->quit();
}

void MainWindow::on_intensity_valueChanged(int value)
{
    float intensity = (float)(value);
    ui->intensitylabel->setText(QString::number(intensity,'f',1));
    ui->translucentMaterials->setLightIntensity(intensity);
}

void MainWindow::timeMeasurement(int millis)
{
   ui->fpslabel->setText(QString("Median time frame (ms): ").append(QString::number(millis)));
}

void MainWindow::keyPressEvent(QKeyEvent * e)
{
    ui->translucentMaterials->keyPressEvent(e);
    QMainWindow::keyPressEvent(e);
}

void MainWindow::keyReleaseEvent(QKeyEvent * e)
{
    ui->translucentMaterials->keyReleaseEvent(e);
    QMainWindow::keyReleaseEvent(e);
}

void MainWindow::radiusChanged(float value)
{

    ui->translucentMaterials->getParameters()->circleradius = value;
}

void MainWindow::samplesChanged(float value)
{
    if(value != ui->translucentMaterials->getParameters()->samples)
    {
        ui->translucentMaterials->getParameters()->samples = (int) value;
        //ui->translucentMaterials->getParameters()->currentFlags |= TranslucentParameters::SAMPLES_CHANGED;
    }
}

void MainWindow::maxsamplesChanged(float value)
{
    if(value != ui->translucentMaterials->getParameters()->maxsamples)
    {
        ui->translucentMaterials->getParameters()->maxsamples = (int) value;
        ui->translucentMaterials->getParameters()->currentFlags |= TranslucentParameters::SAMPLES_CHANGED;
    }
}

void MainWindow::epsilonGBufferChanged(float value)
{
    ui->translucentMaterials->getParameters()->epsilon_gbuffer = value;
}

void MainWindow::epsilonCombinationChanged(float value)
{
    ui->translucentMaterials->getParameters()->epsilon_combination = value;
}

void MainWindow::shadowBiasChanged(float value)
{
    ui->translucentMaterials->getParameters()->shadow_bias = value;
}

void MainWindow::LODChanged(float value)
{
    ui->translucentMaterials->getParameters()->LOD = value;
}

void MainWindow::achanged(float value)
{
    ui->translucentMaterials->getParameters()->a = value;
}

void MainWindow::bchanged(float value)
{
    ui->translucentMaterials->getParameters()->b = value;
}

void MainWindow::gammaChanged(float value)
{
    ui->translucentMaterials->getParameters()->gamma = value;
}

void MainWindow::modelChanged(QString &newModel)
{
    GLGraphics::ThreeDObject * obj = ui->translucentMaterials->setObject(newModel);
    ui->objTest->setObject(obj);
}

void MainWindow::on_cubemapVisible_toggled(bool checked)
{
    ui->translucentMaterials->getParameters()->debugOverlayVisible = checked;
}

void MainWindow::on_jensenbutton_clicked()
{
    ui->translucentMaterials->setRenderMode(TranslucentMaterials::RenderMode::DRAW_JENSEN);
}

void MainWindow::on_directionalbutton_clicked()
{
     ui->translucentMaterials->setRenderMode(TranslucentMaterials::RenderMode::DRAW_DIRECTIONAL);
}

void MainWindow::on_environment_toggled(bool checked)
{
    ui->translucentMaterials->getParameters()->environment = checked;
}
