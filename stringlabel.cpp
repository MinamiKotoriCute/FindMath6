#include "stringlabel.h"

#include <QPainter>

StringLabel::StringLabel(QWidget *parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    resize(parent->width()-2, parent->height()-2);

    pixmap = QPixmap(size());
    clean();
}

void StringLabel::clean(){
    pixmap.fill(Qt::transparent);
    setPixmap(pixmap);
}

void StringLabel::drawString(QString str, QRect rect, QColor color, unsigned int PenSize, unsigned int PixelSize){
    QPainter painter;
    painter.begin(&pixmap);
    painter.setPen(QPen(color, PenSize));
    QFont font;
    font.setPixelSize(PixelSize);
    painter.setFont(font);
    painter.drawText(rect, str);
    painter.end();

    setPixmap(pixmap);
}
