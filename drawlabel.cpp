#include "drawlabel.h"

#include <QPainter>
#include <QDebug>

DrawLabel::DrawLabel(QWidget *parent)
{
    mode = 0;
    focusWidth = 0;
    zoom = 100;

    setAttribute(Qt::WA_TranslucentBackground, true);
    resize(parent->width()-2, parent->height()-2);
    focusRect.setLeft(size().width()/2);
    focusRect.setTop(size().height()*3/8);
    focusRect.setRect(size().width()/4, size().height()*3/8, size().width()/2, size().height()/4);

    pixmap = QPixmap(size());

    updateScreen();
}

void DrawLabel::clean(){
    pixmap.fill(Qt::transparent);
    setPixmap(pixmap);
}

void DrawLabel::zoomChange(int value){
    if(value<30)
        return;

    zoom = value;
    updateScreen();
}

void DrawLabel::focusWidthChange(int value){
    if(focusRect.left()-value<0 || focusRect.right()+value>width())
        return;

    focusWidth = value;
    updateScreen();
}

void DrawLabel::updateScreen(){
    clean();

    if(mode==0){
        const unsigned int length = 30;
        QPainter painter;
        painter.begin(&pixmap);
        painter.setPen(QPen(Qt::red, 5));
        painter.drawLine(focusRect.left()-focusWidth, focusRect.top(), focusRect.left()-focusWidth + length, focusRect.top());
        painter.drawLine(focusRect.left()-focusWidth, focusRect.top(), focusRect.left()-focusWidth, focusRect.top() + length);
        painter.drawLine(focusRect.right()+focusWidth, focusRect.top(), focusRect.right()+focusWidth - length, focusRect.top());
        painter.drawLine(focusRect.right()+focusWidth, focusRect.top(), focusRect.right()+focusWidth, focusRect.top() + length);
        painter.drawLine(focusRect.left()-focusWidth, focusRect.bottom(), focusRect.left()-focusWidth + length, focusRect.bottom());
        painter.drawLine(focusRect.left()-focusWidth, focusRect.bottom(), focusRect.left()-focusWidth, focusRect.bottom() - length);
        painter.drawLine(focusRect.right()+focusWidth, focusRect.bottom(), focusRect.right()+focusWidth - length, focusRect.bottom());
        painter.drawLine(focusRect.right()+focusWidth, focusRect.bottom(), focusRect.right()+focusWidth, focusRect.bottom() - length);
        painter.end();
    }
    else if(mode==1){
        const unsigned int XYSize = 10;
        const unsigned int unitLength = 3;
        QPainter painter;
        painter.begin(&pixmap);
        painter.setPen(QPen(Qt::black, XYSize));
        painter.drawLine(0, height()/2, width(), height()/2);   //x
        painter.drawLine(width()/2, 0, width()/2, height());    //y

        painter.setPen(QPen(Qt::black, 1)); //單位長
        if(zoom>1){
            for(int i=width()/2;i>0;i-=zoom)
                painter.drawLine(i, height()/2-unitLength-XYSize, i, height()/2+unitLength+XYSize);
            for(int i=width()/2;i<width();i+=zoom)
                painter.drawLine(i, height()/2-unitLength-XYSize, i, height()/2+unitLength+XYSize);
            for(int i=height()/2;i>0;i-=zoom)
                painter.drawLine(width()/2-unitLength-XYSize, i, width()/2+unitLength+XYSize, i);
            for(int i=height()/2;i<height();i+=zoom)
                painter.drawLine(width()/2-unitLength-XYSize, i, width()/2+unitLength+XYSize, i);
        }

        painter.setPen(QPen(Qt::blue, 10));
        for(double x=-1000;x<1000;x++){
            Calculate a(mathline);
            Calculate b(mathline);
            double y=zoom*a.ask(x/zoom);
            double i=x+1;
            double j=zoom*b.ask(i/zoom);
            if(y<1000&&j>-1000){
                painter.drawLine(x+width()/2, -y+height()/2, i+width()/2, -j+height()/2);
                //line(matWillReturn,cvPoint(x+cols,-y+rows),cvPoint(i+cols,-j+rows),CV_RGB(0,0,255),3);
            }
        }
        painter.end();
    }

    setPixmap(pixmap);
}

void DrawLabel::drawMathLine(MathLine *mathline){
    this->mathline = mathline;
    mode=1;
    updateScreen();
}

void DrawLabel::setMode(unsigned int newMode){
    mode = newMode;
    updateScreen();
}
