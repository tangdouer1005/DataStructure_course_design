#ifndef ADD_EVENT_WINDOW_H
#define ADD_EVENT_WINDOW_H
#include "ui_add_event_window.h"
#include <QMainWindow>

class add_event_window : public QMainWindow
{
    Q_OBJECT

public:
    add_event_window(QWidget *parent = nullptr);
    ~add_event_window();

    Ui_add_event_window *ui;
};
#endif