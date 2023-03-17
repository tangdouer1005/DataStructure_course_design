#include "DataStructure_course_design.h"

DataStructure_course_design::DataStructure_course_design(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_DataStructure_course_design)
{
    ui->setupUi(this);
    // ui->hello_qt_world->setText("hello_qt_world");
    ui->hello_qt_world->setText("hello_qt_world");
}

DataStructure_course_design::~DataStructure_course_design()
{
    delete ui;
}