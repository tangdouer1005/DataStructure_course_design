#include "navigation_window.h"

navigation_window::navigation_window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_navigation_window)
{
    ui->setupUi(this);
    this->setFixedSize(959, 707);
    ui->label_map->show();
}

navigation_window::~navigation_window()
{
    delete ui;
}