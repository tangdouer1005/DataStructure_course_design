#include "login_window.h"

login_window::login_window(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_login_window)
{
    ui->setupUi(this);
    this->setFixedSize(404, 286);
    QFile file("./data/id_n_password.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        // my_debugger->out("文件打开失败");
    }
    while (!file.atEnd())
    {
        QByteArray array = file.readLine();
        QString id = QString(array);
        QByteArray array_2 = file.readLine();
        QString password = QString(array_2);
        id = id.mid(4);
        password = password.mid(4);
        // id.replace(id.length() - 1, 1, "\t");

        // password.replace(password.length() - 1, 1, "\t");
        id.remove(id.length() - 1, 1);
        password.remove(password.length() - 1, 1);
        id2password[id] = password;
        // my_debugger->out(id);
        // my_debugger->out(password);
    }
}

login_window::~login_window()
{
    delete ui;
}