#ifndef ADVANCEDWIDGET_H
#define ADVANCEDWIDGET_H

#include <QWidget>
#include <QComboBox>
#include <QLabel>
#include <QCheckBox>
#include <QLineEdit>

class AdvancedWidget : public QWidget
{
    Q_OBJECT
public:
    explicit AdvancedWidget(QList<QSize> resolution, QWidget *parent = 0);
    inline QComboBox *addressResolutionComboBox() const;
    inline QCheckBox *addressOpencvContoursPhotoCheckBox() const;
    inline QLineEdit *addressAnswerLineEdit() const;

signals:

public slots:

private:
    QComboBox *resolutionComboBox;
    QCheckBox *opencvContoursPhotoCheckBox;
    QLineEdit *answerLineEdit;
};

inline QComboBox *AdvancedWidget::addressResolutionComboBox() const
{return resolutionComboBox;}

inline QCheckBox *AdvancedWidget::addressOpencvContoursPhotoCheckBox() const
{return opencvContoursPhotoCheckBox;}

inline QLineEdit *AdvancedWidget::addressAnswerLineEdit() const
{return answerLineEdit;}

#endif // ADVANCEDWIDGET_H
