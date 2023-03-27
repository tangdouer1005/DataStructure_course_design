#pragma once
#include "ui_login_window.h"
#include <QMainWindow>
#include <QFile>
#include <QString>
#include <map>
class login_window : public QMainWindow
{
    Q_OBJECT

public:
    login_window(QWidget *parent = nullptr);
    ~login_window();

    std::map<QString, QString> id2password;
    Ui_login_window *ui;
};