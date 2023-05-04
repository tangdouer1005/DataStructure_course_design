#pragma once
#include "ui_add_dairy_event_window.h"
#include <QMainWindow>

class add_dairy_event_window : public QMainWindow
{
    Q_OBJECT

public:
    add_dairy_event_window(QWidget *parent = nullptr);
    ~add_dairy_event_window();

    Ui_add_dairy_event_window *ui;
};