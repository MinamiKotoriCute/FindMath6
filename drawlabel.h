#ifndef DRAWLABEL_H
#define DRAWLABEL_H

#include <QLabel>

#include "calculate.h"

class DrawLabel : public QLabel
{
    Q_OBJECT
public:
    explicit DrawLabel(QWidget *parent = 0);
    void clean();
    void drawMathLine(MathLine *mathline);
    void setMode(unsigned int newMode);
    inline QRect getFocusRect() const;

signals:

public slots:
    void focusWidthChange(int value);
    void zoomChange(int value);

private:
    void updateScreen();
    unsigned int mode;
    int focusWidth, zoom;

    QPixmap pixmap;
    QRect focusRect;
    MathLine *mathline;
};

inline QRect DrawLabel::getFocusRect() const
{return QRect(focusRect.left()-focusWidth, focusRect.top(), focusRect.width()+2*focusWidth, focusRect.height());}

#endif // DRAWLABEL_H
