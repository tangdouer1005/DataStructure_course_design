#pragma once
#include "ui_DataStructure_course_design.h"
#include "debug_label.h"
#include <QMainWindow>
#include "navigation_window.h"
#include "alarm_window.h"
class DataStructure_course_design : public QMainWindow
{
    Q_OBJECT

public:
    debug_label *my_debugger;
    navigation_window *my_navigation;
    alarm_window *my_alarm;
    DataStructure_course_design(QWidget *parent = nullptr);
    ~DataStructure_course_design();

private:
    Ui_DataStructure_course_design *ui;
};