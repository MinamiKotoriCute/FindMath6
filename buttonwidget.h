#ifndef BUTTONWIDGET_H
#define BUTTONWIDGET_H

#include <QWidget>
#include <QSlider>
#include <QLabel>
#include <QPushButton>
#include <QColorDialog>

#include "penwidthlabel.h"

class ButtonWidget : public QWidget
{
    Q_OBJECT
public:
    explicit ButtonWidget(QWidget *parent = 0);
    void setButtonStat(unsigned int newStat);

    inline QSlider* cameraSliderAddress() const;
    inline QSlider* addressZoomSlider() const;
    inline QPushButton* drawButtonAddress() const;
    inline QPushButton* addressResetButton() const;
    inline QPushButton* addressClearButton() const;
    inline PenWidthLabel *addressPenWidthLabel() const;
    inline QColorDialog *addressColorDialog() const;


signals:
    void scribbleAreaCanDraw(bool);

public slots:
    void buttonUpdate();

private slots:
    void nameButton_clicked();
    void exitButton_clicked();
    void drawButton_clicked();
    void nextButton_clicked();
    void resetButton_clicked();
    void modeButton_clicked();
    void colorButton_clicked();
    void widthButton_clicked();
    void clearButton_clicked();

private:
    QSlider *cameraSlider, *zoomSlider;
    QPushButton *nameButton, *exitButton, *drawButton, *nextButton, *resetButton,
                *modeButton, *colorButton, *widthButton, *clearButton;
    QColorDialog *colorDialog;

    PenWidthLabel *penWidthLabel;
    unsigned int buttonStat;
};

inline QSlider* ButtonWidget::cameraSliderAddress() const
{return cameraSlider;}

inline QSlider* ButtonWidget:: addressZoomSlider() const
{return zoomSlider;}

inline QPushButton* ButtonWidget::drawButtonAddress() const
{return drawButton;}

inline QPushButton* ButtonWidget::addressResetButton() const
{return resetButton;}

inline PenWidthLabel *ButtonWidget::addressPenWidthLabel() const
{return penWidthLabel;}

inline QPushButton* ButtonWidget::addressClearButton() const
{return clearButton;}

inline QColorDialog *ButtonWidget::addressColorDialog() const
{return colorDialog;}

#endif // BUTTONWIDGET_H
