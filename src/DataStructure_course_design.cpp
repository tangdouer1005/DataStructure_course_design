#include "DataStructure_course_design.h"

DataStructure_course_design::DataStructure_course_design(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_DataStructure_course_design)
{
    ui->setupUi(this);
    this->setFixedSize(1106, 556);
    // ui->hello_qt_world->setText("hello_qt_world");
    // ui->hello_qt_world->setText("hello_qt_world");

    my_debugger = new debug_label(this);
    my_debugger->show();
    my_debugger->out("hello my_debugger!");
    my_debugger->out("you can do it! my_debugger!");
}

DataStructure_course_design::~DataStructure_course_design()
{
    delete ui;
}