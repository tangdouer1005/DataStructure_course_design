#include "navigation_window.h"

navigation_window::navigation_window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_navigation_window)
{
    ui->setupUi(this);
}

navigation_window::~navigation_window()
{
    delete ui;
}