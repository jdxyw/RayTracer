#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QImage>
#include <QPainter>

#include <opencv/cv.h>
#include <opencv/cxcore.h>
#include <opencv/highgui.h>

#include "common.h"
#include "raytracer.h"
#include "scene.h"
#include "surface.h"

using namespace jdxyw::RayTracer;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::paintEvent(QPaintEvent *)
{
    Surface* surface=new Surface(800,600);
    Pixel* buffer=surface->GetBuffer();
    surface->Clear( 0 );
    Engine* tracer = new Engine();
    tracer->GetScene()->InitScene();
    tracer->SetTarget( surface->GetBuffer(), 800, 600 );
    tracer->InitRender();
    tracer->Render();

//    CvSize size;
//    size.height = 600 ;
//    size.width = 800;
//    IplImage* ipl_image_p = cvCreateImageHeader(size, IPL_DEPTH_32F, 1);
//    //ipl_image_p->imageData = buffer;
//    char *img=ipl_image_p->imageData;
//    for(int i=0;i<480000;i++)
//    {
//        img[3*i]=0x000000FF & buffer[i];
//        img[3*i+1]=0x0000FF00 & buffer[i];
//        img[3*i+2]=0x00FF0000 & buffer[i];
//    }

//    ipl_image_p->imageDataOrigin = ipl_image_p->imageData;

//    QImage image((uchar *)ipl_image_p->imageData,ipl_image_p->width,ipl_image_p->height,ipl_image_p->widthStep,QImage::Format_RGB888);
//    image=image.rgbSwapped();
    QImage image(800, 600, QImage::Format_RGB32);
    for(int i=0;i<600;i++)
    {
        for(int j=0;j<800;j++)
        {
            int r=(buffer[i*800+j]>>16) & 0x000000FF;
            int g=(buffer[i*800+j]>>8) & 0x000000FF;
            int b=0x000000FF & buffer[i*800+j];
            image.setPixel(j,i,qRgb(r,g,b));
        }
    }
    QPainter painter(this);
    painter.drawImage(rect(),image,image.rect());
}
