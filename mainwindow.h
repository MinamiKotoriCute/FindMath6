#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "background.h"
#include "buttonwidget.h"
#include "drawlabel.h"
#include "plugin.h"
#include "advancedwidget.h"
#include "stringlabel.h"
#include "scribblearea.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void show_can_not_draw();

    Background *background;
    ButtonWidget *buttonWidget;
    DrawLabel *drawLabel;
    AdvancedWidget *advancedWidget;
    StringLabel *stringLabel;
    ScribbleArea *scribbleArea;

    Plugin *plugin;

public slots:
    void readToDraw();
    void reset();
};

#endif // MAINWINDOW_H
