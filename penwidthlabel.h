#ifndef PENWIDTHLABEL_H
#define PENWIDTHLABEL_H

#include <QLabel>
#include <QPushButton>

class PenWidthLabel : public QLabel
{
    Q_OBJECT
public:
    explicit PenWidthLabel(QWidget *parent = 0);

signals:
    void setPenWidth(int);

public slots:

private slots:
    void smallButton_clicked();
    void normalButton_clicked();
    void bigButton_clicked();

private:
    QPushButton *smallButton, *normalButton, *bigButton;
};

#endif // PENWIDTHLABEL_H
