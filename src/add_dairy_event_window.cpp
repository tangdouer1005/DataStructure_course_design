#include "add_dairy_event_window.h"

add_dairy_event_window::add_dairy_event_window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_add_dairy_event_window)
{
    ui->setupUi(this);
    this->setFixedSize(303, 298);

    ui->week_spinbox->setRange(1, 18);
    ui->day_spinbox->setRange(1, 7);
    ui->time_spinbox->setRange(6, 21);
}

add_dairy_event_window::~add_dairy_event_window()
{
    delete ui;
}