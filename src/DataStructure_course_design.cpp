#include "DataStructure_course_design.h"

DataStructure_course_design::DataStructure_course_design(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui_DataStructure_course_design)
{
    ui->setupUi(this);
}

DataStructure_course_design::~DataStructure_course_design()
{
    delete ui; 
}