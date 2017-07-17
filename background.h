#ifndef BACKGROUND_H
#define BACKGROUND_H

#include <QLabel>
#include <QCamera>
#include <QCameraImageCapture>
#include <QTimer>

class Background : public QLabel
{
    Q_OBJECT

public:
    Background(QWidget *parent = 0);

    inline QImage getCameraImage() const;
    inline QCamera* cameraAddress() const;

public slots:
    void setResolution(int n);

private slots:
    void displayImage(int id, QImage image);
    void timer1_timeout();

private:
    QCamera *camera;
    QCameraImageCapture *imageCapture;
    QTimer *timer1;
    QImage image;
};

inline QImage Background::getCameraImage() const
{return image.scaled(size(), Qt::IgnoreAspectRatio);}

inline QCamera* Background::cameraAddress() const
{return camera;}

#endif // BACKGROUND_H
