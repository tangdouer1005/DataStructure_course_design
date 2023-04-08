#include "showevent_window.h"

showevent_window::showevent_window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_showevent_window)
{
    ui->setupUi(this);
    this->setFixedSize(491, 431);

    ui->show_something->setFixedSize(800, 600);
    ui->show_something->setFont(QFont(tr("Consolas"), 14));
}

showevent_window::~showevent_window()
{
    delete ui;
}