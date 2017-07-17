#include "penwidthlabel.h"

#include <QVBoxLayout>

PenWidthLabel::PenWidthLabel(QWidget *parent) : QLabel(parent)
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setPixmap(QPixmap(":/image/box.png"));

    QVBoxLayout *vb1 = new QVBoxLayout;
    smallButton = new QPushButton;
    connect(smallButton, SIGNAL(clicked(bool)), this, SLOT(smallButton_clicked()));
    smallButton->setFlat(true);
    smallButton->setIconSize(QSize(150, 50));
    smallButton->setIcon(QIcon(QPixmap(":/image/small.png")));
    vb1->addWidget(smallButton);

    normalButton = new QPushButton;
    connect(normalButton, SIGNAL(clicked(bool)), this, SLOT(normalButton_clicked()));
    normalButton->setFlat(true);
    normalButton->setIconSize(QSize(150, 50));
    normalButton->setIcon(QIcon(QPixmap(":/image/normal.png")));
    vb1->addWidget(normalButton);

    bigButton = new QPushButton;
    connect(bigButton, SIGNAL(clicked(bool)), this, SLOT(bigButton_clicked()));
    bigButton->setFlat(true);
    bigButton->setIconSize(QSize(150, 50));
    bigButton->setIcon(QIcon(QPixmap(":/image/big.png")));
    vb1->addWidget(bigButton);

    setLayout(vb1);
}

void PenWidthLabel::smallButton_clicked(){
    emit setPenWidth(10);
    setHidden(true);
}

void PenWidthLabel::normalButton_clicked(){
    emit setPenWidth(20);
    setHidden(true);
}

void PenWidthLabel::bigButton_clicked(){
    emit setPenWidth(25);
    setHidden(true);
}
