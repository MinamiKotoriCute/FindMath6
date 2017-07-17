#include "buttonwidget.h"

#include <QVBoxLayout>
#include <QCoreApplication>

ButtonWidget::ButtonWidget(QWidget *parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    //mainWidget->setWindowState(Qt::WindowFullScreen);
    buttonStat = 0;

    colorDialog = new QColorDialog;
    colorDialog->setWindowFlags(Qt::Dialog | Qt::FramelessWindowHint);

    QVBoxLayout *vb1 = new QVBoxLayout;
    vb1->addStretch(1);
    cameraSlider = new QSlider(Qt::Vertical);
    //connect(cameraSlider, SIGNAL(valueChanged(int)), recognize, SLOT(rectChange_valueChanged(int)));
    cameraSlider->setRange(-100, 100);
    cameraSlider->setValue(0);
    vb1->addWidget(cameraSlider, 3, Qt::AlignHCenter);

    zoomSlider = new QSlider(Qt::Vertical);
    //connect(zoomSlider, SIGNAL(valueChanged(int)), recognize, SLOT(drawzoom_valueChanged(int)));
    zoomSlider->setRange(30, 200);
    zoomSlider->setValue(100);
    vb1->addWidget(zoomSlider, 3, Qt::AlignHCenter);
    vb1->addStretch(1);

    nameButton = new QPushButton;
    connect(nameButton, SIGNAL(clicked(bool)), this, SLOT(nameButton_clicked()));
    nameButton->setFlat(true);
    nameButton->setIconSize(QSize(150, 120));
    nameButton->setIcon(QIcon(":/image/EqnEye.png"));;
    vb1->addWidget(nameButton);

    exitButton = new QPushButton;
    connect(exitButton, SIGNAL(clicked(bool)), this, SLOT(exitButton_clicked()));
    exitButton->setFlat(true);
    exitButton->setIconSize(QSize(150, 120));
    exitButton->setIcon(QIcon(":/image/close.png"));;
    vb1->addWidget(exitButton);

    QVBoxLayout *vb2 = new QVBoxLayout;
    modeButton = new QPushButton;
    connect(modeButton, SIGNAL(clicked(bool)), this, SLOT(modeButton_clicked()));
    modeButton->setFlat(true);
    modeButton->setIconSize(QSize(150, 120));
    modeButton->setIcon(QIcon(":/image/general mode.png"));;
    vb2->addWidget(modeButton, 0, Qt::AlignRight);

    vb2->addStretch(1);
    drawButton = new QPushButton;
    connect(drawButton, SIGNAL(clicked(bool)), this, SLOT(drawButton_clicked()));
    drawButton->setFlat(true);
    drawButton->setIconSize(QSize(150, 120));
    drawButton->setIcon(QIcon(":/image/capture_on.png"));;
    vb2->addWidget(drawButton);

    nextButton = new QPushButton;
    connect(nextButton, SIGNAL(clicked(bool)), this, SLOT(nextButton_clicked()));
    nextButton->setFlat(true);
    nextButton->setIconSize(QSize(150, 120));
    nextButton->setIcon(QIcon(":/image/next_on.png"));;
    vb2->addWidget(nextButton);

    resetButton = new QPushButton;
    connect(resetButton, SIGNAL(clicked(bool)), this, SLOT(resetButton_clicked()));
    resetButton->setFlat(true);
    resetButton->setIconSize(QSize(150, 120));
    resetButton->setIcon(QIcon(":/image/re.png"));;
    vb2->addWidget(resetButton);

    QHBoxLayout *hb21 = new QHBoxLayout;
    penWidthLabel = new PenWidthLabel;
    hb21->addWidget(penWidthLabel);

    widthButton = new QPushButton;
    connect(widthButton, SIGNAL(clicked(bool)), this, SLOT(widthButton_clicked()));
    widthButton->setFlat(true);
    widthButton->setIconSize(QSize(150, 120));
    widthButton->setIcon(QIcon(":/image/pen.png"));
    hb21->addWidget(widthButton);
    vb2->addLayout(hb21);

    colorButton = new QPushButton;
    connect(colorButton, SIGNAL(clicked(bool)), this, SLOT(colorButton_clicked()));
    colorButton->setFlat(true);
    colorButton->setIconSize(QSize(150, 120));
    colorButton->setIcon(QIcon(":/image/color.png"));;
    vb2->addWidget(colorButton, 0, Qt::AlignRight);

    clearButton = new QPushButton;
    connect(clearButton, SIGNAL(clicked(bool)), this, SLOT(clearButton_clicked()));
    clearButton->setFlat(true);
    clearButton->setIconSize(QSize(150, 120));
    clearButton->setIcon(QIcon(":/image/clear.png"));;
    vb2->addWidget(clearButton, 0, Qt::AlignRight);

    QHBoxLayout *hb1 = new QHBoxLayout;
    hb1->addLayout(vb1);
    hb1->addStretch(1);
    hb1->addLayout(vb2);

    buttonUpdate();

    setLayout(hb1);
    resize(parent->width()-2, parent->height()-2);
    //cameraSlider->resize(nameButton->width(), cameraSlider->height());
}

void ButtonWidget::buttonUpdate(){
    if(buttonStat==0){
        drawButton->setEnabled(true);
        drawButton->setHidden(false);
        nextButton->setEnabled(false);   //byebye
        nextButton->setHidden(true);
        resetButton->setEnabled(false);
        resetButton->setHidden(false);
        modeButton->setHidden(true);
        colorButton->setHidden(true);
        widthButton->setHidden(true);
        clearButton->setHidden(true);
        penWidthLabel->setHidden(true);
        cameraSlider->setHidden(false);
        zoomSlider->setHidden(true);
        emit scribbleAreaCanDraw(false);
    }
    else if(buttonStat==1){
        drawButton->setEnabled(false);
        drawButton->setHidden(false);
        nextButton->setEnabled(true);
        nextButton->setHidden(true);
        resetButton->setEnabled(true);
        resetButton->setHidden(false);
        modeButton->setHidden(false);
        modeButton->setIcon(QIcon(":/image/general mode.png"));
        colorButton->setHidden(true);
        widthButton->setHidden(true);
        clearButton->setHidden(true);
        penWidthLabel->setHidden(true);
        cameraSlider->setHidden(true);
        zoomSlider->setHidden(false);
        emit scribbleAreaCanDraw(false);
    }
    else if(buttonStat==2){
        drawButton->setEnabled(false);
        drawButton->setHidden(true);
        nextButton->setEnabled(true);
        nextButton->setHidden(true);
        resetButton->setEnabled(true);
        resetButton->setHidden(true);
        modeButton->setHidden(false);
        modeButton->setIcon(QIcon(":/image/canvas mode.png"));
        colorButton->setHidden(false);
        widthButton->setHidden(false);
        clearButton->setHidden(false);
        penWidthLabel->setHidden(true);
        cameraSlider->setHidden(true);
        zoomSlider->setHidden(true);
        emit scribbleAreaCanDraw(true);
    }
}

void ButtonWidget::nameButton_clicked(){

}

void ButtonWidget::exitButton_clicked(){
    QCoreApplication::quit();
}

void ButtonWidget::drawButton_clicked(){

}

void ButtonWidget::nextButton_clicked(){

}

void ButtonWidget::resetButton_clicked(){

}

void ButtonWidget::modeButton_clicked(){
    if(buttonStat==1){
        buttonStat=2;
        buttonUpdate();
    }
    else if(buttonStat==2){
        buttonStat=1;
        buttonUpdate();
    }
}

void ButtonWidget::colorButton_clicked(){
    colorDialog->exec();
}

void ButtonWidget::widthButton_clicked(){
    penWidthLabel->show();
}

void ButtonWidget::clearButton_clicked(){

}

void ButtonWidget::setButtonStat(unsigned int newStat){
    buttonStat = newStat;
    buttonUpdate();
}
