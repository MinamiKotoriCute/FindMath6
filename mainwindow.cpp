#include "mainwindow.h"

#include <QDebug>
#include <QGraphicsScene>
#include <QGraphicsView>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setWindowState(Qt::WindowFullScreen);   //全螢幕

    background = new Background(this);
    buttonWidget = new ButtonWidget(this);
    drawLabel = new DrawLabel(this);
    advancedWidget = new AdvancedWidget(background->cameraAddress()->supportedViewfinderResolutions());
    stringLabel = new StringLabel(this);
    scribbleArea = new ScribbleArea(this);

    plugin = new Plugin;

    advancedWidget->show();

    connect(buttonWidget->cameraSliderAddress(), SIGNAL(valueChanged(int)), drawLabel, SLOT(focusWidthChange(int)));
    connect(buttonWidget->addressZoomSlider(), SIGNAL(valueChanged(int)), drawLabel, SLOT(zoomChange(int)));
    connect(buttonWidget->drawButtonAddress(), SIGNAL(clicked(bool)), this, SLOT(readToDraw()));
    connect(buttonWidget->addressResetButton(), SIGNAL(clicked(bool)), this, SLOT(reset()));
    connect(buttonWidget, SIGNAL(scribbleAreaCanDraw(bool)), scribbleArea, SLOT(setPenCanDraw(bool)));
    connect(buttonWidget->addressPenWidthLabel(), SIGNAL(setPenWidth(int)), scribbleArea, SLOT(setPenWidth(int)));
    connect(buttonWidget->addressClearButton(), SIGNAL(clicked(bool)), scribbleArea, SLOT(clearImage()));
    connect(buttonWidget->addressColorDialog(), SIGNAL(colorSelected(QColor)), scribbleArea, SLOT(setPenColor(QColor)));
    connect(advancedWidget->addressResolutionComboBox(), SIGNAL(currentIndexChanged(int)), background, SLOT(setResolution(int)));
    connect(advancedWidget->addressOpencvContoursPhotoCheckBox(), SIGNAL(toggled(bool)), plugin, SLOT(setShowOpencvContoursPhoto(bool)));

    QGraphicsScene *scene = new QGraphicsScene; //畫面的世界
    scene->addWidget(background);
    scene->addWidget(drawLabel);
    scene->addWidget(stringLabel);
    scene->addWidget(scribbleArea);
    scene->addWidget(buttonWidget);

    QGraphicsView *view = new QGraphicsView(scene); //看世界的地方

    setCentralWidget(view);
}

MainWindow::~MainWindow()
{

}

void MainWindow::reset(){
    buttonWidget->setButtonStat(0);
    drawLabel->setMode(0);
}

void MainWindow::readToDraw(){
    //stringLabel->drawString("Try again.", QRect(this->width()/2 - 200, this->height()/2 - 100, this->width()/2 + 200, this->height()/2 + 100));

    MathLine *m = plugin->opencvSegmentation(background->getCameraImage().copy(drawLabel->getFocusRect()));

    if(m->analysis()){
        show_can_not_draw();
        qDebug() << "this photo can't analysis.";
        return;
    }
    m->addBrackets();

    qDebug() << m->show();
    advancedWidget->addressAnswerLineEdit()->setText(m->show());

    MathLine *m2 = m->allInOne();
    if(m2->elements().size()<1 || m2->elements().size()>100){
        show_can_not_draw();
        qDebug() << "this photo can't draw.";
        return;
    }
    if(m2->elements()[0]->type()==3 || m2->elements()[m2->elements().size()-1]->type()==3){
        show_can_not_draw();
        qDebug() << "this photo can't draw 2.";
        return;
    }
    for(int i=0;i<m2->elements().size();i++)
        if(m2->elements()[i]->type()==-1){
            show_can_not_draw();
            qDebug() << "this photo can't draw 3.";
            return;
        }

    if(!m2->canDraw()){
        show_can_not_draw();
        qDebug() << "this photo can't draw 4.";
        return;
    }

    buttonWidget->setButtonStat(1);
    drawLabel->drawMathLine(m2);
}

void MainWindow::show_can_not_draw(){
    stringLabel->drawString("Try again.", QRect(this->width()/2 - 200, this->height()/2 - 100, this->width()/2 + 200, this->height()/2 + 100));
    QTimer::singleShot(2000, stringLabel, SLOT(clean()));
}
