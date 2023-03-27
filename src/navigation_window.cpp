#include "navigation_window.h"

navigation_window::navigation_window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_navigation_window)
{
    ui->setupUi(this);
    this->setFixedSize(843, 721);
    ui->label_map->show();
<<<<<<< HEAD
=======

>>>>>>> 035c40f7cc9a78447bbff8592f9b6805f19a8ff1
}

navigation_window::~navigation_window()
{
    delete ui;
}