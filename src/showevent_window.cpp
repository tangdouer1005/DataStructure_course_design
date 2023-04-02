#include "showevent_window.h"

showevent_window::showevent_window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_showevent_window)
{
    ui->setupUi(this);
    this->setFixedSize(434, 612);
}

showevent_window::~showevent_window()
{
    delete ui;
}