#ifndef ADD_EVENT_WINDOW_H
#define ADD_EVENT_WINDOW_H
#include "ui_add_event_window.h"
#include <QMainWindow>
#include "DataStructure_course_design.h"
class DataStructure_course_design;
class add_event_window : public QMainWindow
{
    Q_OBJECT

public:
    add_event_window(QWidget *parent = nullptr);
    ~add_event_window();
    class DataStructure_course_design *father;

    Ui_add_event_window *ui;
private slots:
    void slot_click_confirm_button();
};
#endif