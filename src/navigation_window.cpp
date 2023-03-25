#include "navigation_window.h"

navigation_window::navigation_window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_navigation_window)
{
    ui->setupUi(this);
    QImage map("../res/pic/map.jpg");
    ui->label_map->setPixmap(QPixmap::fromImage(map));
    ui->label_map->show();
}

navigation_window::~navigation_window()
{
    delete ui;
}