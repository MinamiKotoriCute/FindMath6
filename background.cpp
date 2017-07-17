#include "background.h"

#include <QDebug>
#include <QCameraViewfinderSettings>
#include <QCameraInfo>
#include <QMessageBox>

Background::Background(QWidget *parent)
{
    resize(parent->width()-2, parent->height()-2);

    QCameraInfo cameraInfo;
    if(cameraInfo.availableCameras().size() == 0){
        QMessageBox::about(this, "Oops!", "Can't find camera.");
        exit(1);
    }

    camera = new QCamera;
    imageCapture = new QCameraImageCapture(camera);
    connect(imageCapture, SIGNAL(imageCaptured(int,QImage)), this, SLOT(displayImage(int,QImage)));
    camera->start();    //啟動鏡頭

    QCameraViewfinderSettings setting;  //鏡頭設定
    setting.setResolution(camera->supportedViewfinderResolutions()[camera->supportedViewfinderResolutions().size()-1]); //應該是最高解析度
    camera->setViewfinderSettings(setting);

    timer1 = new QTimer;
    connect(timer1, SIGNAL(timeout()), this, SLOT(timer1_timeout()));

    timer1->start(33);
}

void Background::timer1_timeout(){
    //擷取當下視訊畫面
    imageCapture->capture();
}

void Background::displayImage(int id, QImage image){
    this->image = image;
    setPixmap(QPixmap::fromImage(image).scaled(size(), Qt::IgnoreAspectRatio));
}

void Background::setResolution(int n){
    if(n<0 || n>=camera->supportedViewfinderResolutions().size())
        return;

    QCameraViewfinderSettings setting;  //鏡頭設定
    setting.setResolution(camera->supportedViewfinderResolutions()[n]); //應該是最高解析度
    camera->setViewfinderSettings(setting);
}
