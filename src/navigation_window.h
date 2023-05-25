#ifndef NAVIGATION_WINDOW_H
#define NAVIGATION_WINDOW_H

#include "ui_navigation_window.h"
#include <QMainWindow>
#include <QLabel>
class navigation_window : public QMainWindow
{
    Q_OBJECT

public:
    navigation_window(QWidget *parent = nullptr);
    ~navigation_window();

    Ui_navigation_window *ui;
};
#endif