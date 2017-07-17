#ifndef STRINGLABEL_H
#define STRINGLABEL_H

#include <QLabel>

class StringLabel : public QLabel
{
    Q_OBJECT
public:
    explicit StringLabel(QWidget *parent = 0);
    void drawString(QString str, QRect rect, QColor color = Qt::red, unsigned int PenSize = 10, unsigned int PixelSize = 100);

signals:

public slots:
    void clean();

private:
    QPixmap pixmap;
};

#endif // STRINGLABEL_H
