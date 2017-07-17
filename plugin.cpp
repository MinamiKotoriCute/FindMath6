#include "plugin.h"

#include <QDebug>

Plugin::Plugin()
{
    isShowOpencvContoursPhoto = false;
}

void Plugin::setShowOpencvContoursPhoto(bool s){
    isShowOpencvContoursPhoto = s;
}

MathLine *Plugin::opencvSegmentation(QImage imageIn, QImage *imageOut){
    Mat src = QImageToCvMat(imageIn);

    if(src.empty()){
        qDebug() << "error in Recognize::opencvSegmentation(), Image is empty.";
        return NULL;
    }
    MathLine *mathline = new MathLine;

    Mat src_gray;
    /// Convert image to gray and blur it
    cvtColor( src, src_gray, CV_BGR2GRAY );
    //blur( src_gray, src_gray, Size(3,3) );
    medianBlur( src_gray,src_gray, 5);

    RNG rng(12345);
    Mat threshold_output;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;

    /// Detect edges using Threshold
    threshold( src_gray, threshold_output,0, 255, 8 );
    /// Find contours
    findContours( threshold_output, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );

    /// Approximate contours to polygons + get bounding rects and circles
    vector<vector<Point> > contours_poly( contours.size() );
    vector<Rect> boundRect( contours.size() );
    vector<Point2f>center( contours.size() );
    vector<float>radius( contours.size() );

    for( int i = 0; i < contours.size(); i++ ){
        approxPolyDP( Mat(contours[i]), contours_poly[i], 3, true );
        boundRect[i] = boundingRect( Mat(contours_poly[i]) );
        minEnclosingCircle( (Mat)contours_poly[i], center[i], radius[i] );
    }

    /// Draw polygonal contour + bonding rects + circles

    Mat drawing = Mat::zeros( threshold_output.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ ) {
        Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
        drawContours( drawing, contours_poly, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
        rectangle( drawing, boundRect[i].tl(), boundRect[i].br(), color, 2, 8, 0 );

        Mat roi_img;
        src(boundRect[i]).copyTo(roi_img);


        QString data(tess(roi_img));
        QRect qrect(boundRect[i].x, boundRect[i].y, boundRect[i].width, boundRect[i].height);
        if(data == 0)
            mathline->elementsAdd(new Element(qrect , "$"));
        else
            mathline->elementsAdd(new Element(qrect , data));
        if(i==0)
            continue;
    }

    /// Show in a window
    imageOut = &(cvMatToQImage(drawing).copy());
    drawing.copyTo(ContoursPhoto);
    if(isShowOpencvContoursPhoto)
        imshow("Opencv Contours Photo", ContoursPhoto);

    return mathline;
}

char* Plugin::tess(Mat Mimg02){
    IplImage* img_erode_dilate ;                  //先宣告 IplImage* 型別的img02
    img_erode_dilate = &IplImage(Mimg02);         //Matimg的位置丟給img02

    tesseract::TessBaseAPI api ;
    if (api.Init(NULL, "eng", tesseract::OEM_DEFAULT) )  {
        fprintf(stderr, "Could not initialize tesseract.\n");
        return NULL;
    }
    //?置?像
    api.SetImage((unsigned char*)img_erode_dilate->imageData, img_erode_dilate->width,
        img_erode_dilate->height, img_erode_dilate->nChannels, img_erode_dilate->widthStep);

    //api.SetVariable( "tessedit_char_whitelist", "0123456789." );
    //?得?片?果

    char* outText = api.GetUTF8Text();
    //cout << outText << endl;

    return outText;
}

QImage Plugin::cvMatToQImage( const cv::Mat &inMat )
   {
      switch ( inMat.type() )
      {
         // 8-bit, 4 channel
         case CV_8UC4:
         {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB32 );

            return image;
         }

         // 8-bit, 3 channel
         case CV_8UC3:
         {
            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_RGB888 );

            return image.rgbSwapped();
         }

         // 8-bit, 1 channel
         case CV_8UC1:
         {
            static QVector<QRgb>  sColorTable;

            // only create our color table once
            if ( sColorTable.isEmpty() )
            {
               for ( int i = 0; i < 256; ++i )
                  sColorTable.push_back( qRgb( i, i, i ) );
            }

            QImage image( inMat.data, inMat.cols, inMat.rows, inMat.step, QImage::Format_Indexed8 );

            image.setColorTable( sColorTable );

            return image;
         }

         default:
            qWarning() << "ASM::cvMatToQImage() - cv::Mat image type not handled in switch:" << inMat.type();
            break;
      }

      return QImage();
   }

cv::Mat Plugin::QImageToCvMat( const QImage &inImage, bool inCloneImageData)
{
    switch ( inImage.format() )
    {
        // 8-bit, 4 channel
        case QImage::Format_RGB32:
        {
            cv::Mat  mat( inImage.height(), inImage.width(), CV_8UC4, const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine() );

            return (inCloneImageData ? mat.clone() : mat);
        }

        // 8-bit, 3 channel
        case QImage::Format_RGB888:
        {
            if ( !inCloneImageData )
                qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning since we use a temporary QImage";

            QImage   swapped = inImage.rgbSwapped();

            return cv::Mat( swapped.height(), swapped.width(), CV_8UC3, const_cast<uchar*>(swapped.bits()), swapped.bytesPerLine() ).clone();
        }

        // 8-bit, 1 channel
        case QImage::Format_Indexed8:
        {
            cv::Mat  mat( inImage.height(), inImage.width(), CV_8UC1, const_cast<uchar*>(inImage.bits()), inImage.bytesPerLine() );

            return (inCloneImageData ? mat.clone() : mat);
        }

        default:
        qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
        break;
    }

    return cv::Mat();
}
