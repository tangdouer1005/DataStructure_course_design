#pragma once
#include "ui_DataStructure_course_design.h"
#include "debug_label.h"
#include <QMainWindow>

class DataStructure_course_design : public QMainWindow
{
    Q_OBJECT

public:
    debug_label *my_debugger;
    DataStructure_course_design(QWidget *parent = nullptr);
    ~DataStructure_course_design();

private:
    Ui_DataStructure_course_design *ui;
};