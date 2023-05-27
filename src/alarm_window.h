#ifndef ALARM_WINDOW_H
#define ALARM_WINDOW_H
#include "ui_alarm_window.h"
#include <QMainWindow>
#include "DataStructure_course_design.h"
#include <set>
class DataStructure_course_design;
class alarm_window : public QMainWindow
{
    Q_OBJECT

public:
    alarm_window(QWidget *parent = nullptr);
    ~alarm_window();
    void init_combo();

    class DataStructure_course_design *father;

    std::set<QString> alarming_dairy_event;
    std::set<QString> alarming_temporary_event;

private:
    Ui_alarm_window *ui;
private slots:
    void slot_click_dairy_button();
    void slot_click_temporary_button();
};
#endif