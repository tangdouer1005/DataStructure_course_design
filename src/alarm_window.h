#pragma once
#include "ui_alarm_window.h"
#include <QMainWindow>

class alarm_window : public QMainWindow
{
    Q_OBJECT

public:
    alarm_window(QWidget *parent = nullptr);
    ~alarm_window();

private:
    Ui_alarm_window *ui;
};