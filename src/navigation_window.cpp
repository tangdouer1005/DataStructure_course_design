#include "navigation_window.h"

navigation_window::navigation_window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_navigation_window)
{
    ui->setupUi(this);
    this->setFixedSize(1008, 763);
    // ui->label_map->show();

    // ui->label_map->setScaledContents(true);
}

navigation_window::~navigation_window()
{
    delete ui;
}