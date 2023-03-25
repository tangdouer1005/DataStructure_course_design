#include "login_window.h"

login_window::login_window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_login_window)
{
    ui->setupUi(this);
    this->setFixedSize(404, 286);
}

login_window::~login_window()
{
    delete ui;
}