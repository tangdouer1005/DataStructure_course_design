#pragma once
#include "ui_navigation_window.h"
#include <QMainWindow>

class navigation_window : public QMainWindow
{
    Q_OBJECT

public:
    navigation_window(QWidget *parent = nullptr);
    ~navigation_window();

private:
    Ui_navigation_window *ui;
};