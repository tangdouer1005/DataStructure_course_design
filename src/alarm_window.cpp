#include "alarm_window.h"

alarm_window::alarm_window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_alarm_window)
{
    ui->setupUi(this);
    this->setFixedSize(434, 612);
}

alarm_window::~alarm_window()
{
    delete ui;
}