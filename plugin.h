#ifndef PLUGIN_H
#define PLUGIN_H

#include <QImage>

#include <tesseract/baseapi.h>
#include <leptonica/allheaders.h>

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <opencv/cv.hpp>

#include "mathline.h"

using namespace std;
using namespace cv;

class Plugin : public QObject
{
    Q_OBJECT

public:
    Plugin();

    MathLine* opencvSegmentation(QImage imageIn, QImage *imageOut = 0);
    char* tess(Mat Mimg02); // tesseract負責部分

public slots:
    void setShowOpencvContoursPhoto(bool s);

private:
    cv::Mat QImageToCvMat( const QImage &inImage, bool inCloneImageData = true );
    QImage cvMatToQImage( const cv::Mat &inMat );

    bool isShowOpencvContoursPhoto;
    Mat ContoursPhoto;
};

#endif // PLUGIN_H
