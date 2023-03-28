#pragma once
#include "ui_DataStructure_course_design.h"
#include "debug_label.h"
#include <QMainWindow>
#include "navigation_window.h"
#include "alarm_window.h"
#include "login_window.h"
#include <QImage>
#include <QFile>
#include <vector>
#include <map>
class DataStructure_course_design : public QMainWindow
{
    Q_OBJECT

public:
    debug_label *my_debugger;
    navigation_window *my_navigation;
    alarm_window *my_alarm;
    login_window *my_login;
    // void slot_alarmshow()
    // {
    //     my_debugger->out("slot_alarmshow");
    //     my_alarm->show();
    // }
    // void slot_navigationshow()
    // {
    //     my_debugger->out("slot_navigation_show");
    //     my_navigation->show();
    // }
    // void slot_addshow() {}
    DataStructure_course_design(QWidget *parent = nullptr);
    ~DataStructure_course_design();

private:
    Ui_DataStructure_course_design *ui;
};