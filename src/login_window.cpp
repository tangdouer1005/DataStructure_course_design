#include "login_window.h"

login_window::login_window(QWidget *parent)
    : QMainWindow(parent), father((class DataStructure_course_design *)parent), ui(new Ui_login_window)
{
    ui->setupUi(this);
    this->setFixedSize(404, 286);

    connect(ui->button_login, SIGNAL(clicked()), this, SLOT(slot_login_click()));
    connect(ui->button_register, SIGNAL(clicked()), this, SLOT(slot_register_click()));

    QFile file("./data/id_n_password.txt");
    QFile idFile("./data/id_n_password.txt");
    if (!idFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
    }
    QTextStream idFileIn(&idFile);
    idFileIn.setCodec("UTF-8"); // 确定编码格式
    while (!idFileIn.atEnd())
    {
        QString id, password;
        idFileIn >> id >> password;
        if (id != QString("") && password != QString(""))
        {
            id2password[id] = password;
        }
    }
    idFile.close();
}

login_window::~login_window()
{
    QFile idFile("./data/id_n_password.txt");
    if (!idFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
    }
    QTextStream idFileIn(&idFile);
    for (auto iter : id2password)
    {
        idFileIn << iter.first << QString(" ");
        idFileIn << iter.second << QString("\n");
    }
    idFile.close();
    delete ui;
}

void login_window::slot_login_click()
{
    QString in_id = ui->lineedit_id->text();
    QString in_password = ui->lineedit_password->text();

    if (id2password.count(in_id))
    {
        if (id2password[in_id] == in_password)
        {
            father->show();
            close();
            father->my_timer->start(TIME_UNIT);
        }
        else
        {
            QMessageBox::information(this,
                                     tr("wrong password"),
                                     tr("请输入正确的password"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
            father->my_debugger->out("wrong password");
        }
    }
    else
    {
        QMessageBox::information(this,
                                 tr("wrong id"),
                                 tr("此id不存在"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        father->my_debugger->out("no this id");
    }
}
void login_window::slot_register_click()
{
    QString in_id = ui->lineedit_id->text();
    QString in_password = ui->lineedit_password->text();

    if (id2password.count(in_id))
    {
        if (id2password[in_id] == in_password)
        {
            father->show();
            close();
            father->my_timer->start(TIME_UNIT);
        }
        else
        {
            QMessageBox::information(this,
                                     tr("wrong register"),
                                     tr("此id已存在"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
            father->my_debugger->out("id exit");
        }
    }
    else
    {
        id2password[in_id] = in_password;
        QString filename = in_id;
        id2password.insert({in_id, in_password});
        QFile file(QString("./data/") + filename);

        // 打开文件并检查是否成功
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out.setCodec("UTF-8"); // 确定编码格式
            //  写入数据到文件
            out << in_id << " " << in_password << "\n";
            out << 12 << "\n"
                << QString("计算机网络 ")
                << QString("java ")
                << QString("操作系统 ")
                << QString("计算机组成原理 ")
                << QString("形势与政策 ")
                << QString("毛泽东思想和中国特色社会主义理论体系概论 ")
                << QString("数据库 ")
                << QString("形式语言与自动机 ")
                << QString("数字逻辑 ")
                << QString("计算机网络课程设计 ")
                << QString("计算计组成原理实验 ")
                << QString("数据结构课程设计 ");
            out << "\n0 0"; // 日常事务和临时事务
            // 关闭文件
            file.close();
        }
        else
        {
            father->my_debugger->out("创建文件注册者文件失败");
        }
        QMessageBox::information(this,
                                 tr("succeed register"),
                                 tr("注册成功"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        father->my_debugger->out("register id");
    }
}