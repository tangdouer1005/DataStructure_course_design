#pragma once
#include "ui_login_window.h"
#include <QMainWindow>

class login_window : public QMainWindow
{
    Q_OBJECT

public:
    login_window(QWidget *parent = nullptr);
    ~login_window();

    Ui_login_window *ui;
};