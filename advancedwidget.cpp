#include "advancedwidget.h"

#include <QVBoxLayout>

AdvancedWidget::AdvancedWidget(QList<QSize> resolution, QWidget *parent) : QWidget(parent)
{
    setWindowFlags(Qt::WindowStaysOnTopHint);
    setWindowTitle("Developer Options");

    QVBoxLayout *vb1 = new QVBoxLayout;

    QHBoxLayout *hb1 = new QHBoxLayout;
    QLabel *label1 = new QLabel("resolution");
    hb1->addWidget(label1);
    resolutionComboBox = new QComboBox;
    for(int i=0;i<resolution.size();i++)
        resolutionComboBox->addItem(QString("%1 x %2").arg(resolution[i].width()).arg(resolution[i].height()));
    resolutionComboBox->setCurrentIndex(resolution.size()-1);
    hb1->addWidget(resolutionComboBox);
    vb1->addLayout(hb1);

    opencvContoursPhotoCheckBox = new QCheckBox("Show Opencv Contours Photo");
    vb1->addWidget(opencvContoursPhotoCheckBox);

    QHBoxLayout *hb2 = new QHBoxLayout;
    QLabel *label2 = new QLabel("last answer");
    hb2->addWidget(label2);
    answerLineEdit = new QLineEdit;
    hb2->addWidget(answerLineEdit);
    vb1->addLayout(hb2);

    setLayout(vb1);
}

