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
    if (idFile.open(QIODevice::ReadOnly | QIODevice::Text))
    {
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
    }
    idFile.close();
    QRegExp regExp("[A-Za-z0-9]*");
    QValidator *validator = new QRegExpValidator(regExp, this);
    ui->lineedit_id->setValidator(validator);
    ui->lineedit_password->setValidator(validator);
    ui->lineedit_password->setEchoMode(QLineEdit::Password);
}

login_window::~login_window()
{
    QFile idFile("./data/id_n_password.txt");
    if (idFile.open(QIODevice::WriteOnly | QIODevice::Truncate))
    {
        QTextStream idFileIn(&idFile);
        idFileIn.setCodec("UTF-8"); // 确定编码格式

        for (auto iter : id2password)
        {
            idFileIn << iter.first << QString(" ");
            idFileIn << iter.second << QString("\n");
        }
    }
    else
    {
    }

    idFile.close();
    delete ui;
}

void login_window::slot_login_click()
{
    QString in_id = ui->lineedit_id->text();
    QString in_password = ui->lineedit_password->text();
    if (in_id.size() < 5 || in_id.size() > 12 || in_password.size() < 5 || in_password.size() > 12)
    {
        QMessageBox::information(this,
                                 tr("wrong"),
                                 tr("id和password的长度都要保持在5到12"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        return;
    }
    if (!ui->user_radio->isChecked() && !ui->manager_radio->isChecked())
    {
        QMessageBox::information(this,
                                 tr("wrong"),
                                 tr("请选择一种登录方式"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        return;
    }
    if (ui->manager_radio->isChecked())
    {
        if (in_id == QString("houge"))
        {
            if (in_password == QString("hhhhh"))
            {
                father->my_manager->show();
                this->hide();
                return;
            }
            else
            {
                QMessageBox::information(this,
                                         tr("wrong password"),
                                         tr("请输入正确的password"),
                                         QMessageBox::Ok | QMessageBox::Cancel,
                                         QMessageBox::Ok);
            }
        }
        else
        {
            QMessageBox::information(this,

                                     tr("wrong id"),
                                     tr("请输入正确的id"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
        }
    }
    if (id2password.count(in_id))
    {
        if (id2password[in_id] == in_password)
        {
            QString filename = in_id;

            QFile file(QString("./data/users/") + filename);

            // 打开文件并检查是否成功
            if (file.open(QIODevice::ReadOnly | QIODevice::Text))
            {
                QTextStream fileIn(&file);
                fileIn.setCodec("UTF-8"); // 确定编码格式
                fileIn >> father->user->id >> father->user->password;
                fileIn >> father->user->week >> father->user->day >> father->user->hour;
                int course_num, dairy_num, tem_num, alarm_dairy, alarm_tem;
                fileIn >> course_num;
                while (course_num--)
                {
                    QString t_course;
                    fileIn >> t_course;
                    father->user->courses.push_back(t_course);
                }
                fileIn >> dairy_num;
                while (dairy_num--)
                {
                    QString name, site;
                    int week, day, hour, freq;
                    fileIn >> name >> site >> week >> day >> hour;
                    father->user->dairy_event.push_back({name, site, week, day, hour});
                }
                fileIn >> tem_num;
                while (tem_num--)
                {
                    QString name, site;
                    int week, day, hour;
                    fileIn >> name >> site >> week >> day >> hour;
                    father->user->temporary_event.push_back({name, site, week, day, hour});
                }
                fileIn >> alarm_dairy;
                while (alarm_dairy--)
                {
                    QString name;
                    fileIn >> name;
                    father->my_alarm->alarming_dairy_event.insert(name);
                }
                fileIn >> alarm_tem;
                while (alarm_tem--)
                {
                    QString name;
                    fileIn >> name;
                    father->my_alarm->alarming_temporary_event.insert(name);
                }
                father->my_alarm->init_list();

                file.close();
            }
            else
            {
            }
            father->get_course_event();
            close();
            father->my_alarm->init_combo();
            if (father->user->week == 1 && father->user->day == 1 && father->user->hour == 0)
            {
                father->my_select->init_combobox();
                father->my_select->show();
            }
            else
            {
                father->show();
                father->my_timer->start(TIME_UNIT);
            }
            father->init_log();
            father->my_debugger->out("登陆成功 id:" + in_id);

            QString htmlText = "<p style='font-size:20pt;'>ID: " + in_id.toUtf8() + "</p>";
            htmlText += "<p style='font-size:20pt;'>GRADE: Sophomore</p>";
            htmlText += "<p style='font-size:20pt;'>CLASS: 2021211260</p>";

            QTextCharFormat format;
            format.setFontPointSize(20);

            father->ui->textBrowser->setCurrentCharFormat(format);
            father->ui->textBrowser->setHtml(htmlText);
        }
        else
        {
            QMessageBox::information(this,
                                     tr("wrong password"),
                                     tr("请输入正确的password"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
        }
    }
    else
    {
        QMessageBox::information(this,
                                 tr("wrong id"),
                                 tr("此id不存在"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
    }
}

void login_window::slot_register_click()
{
    QString in_id = ui->lineedit_id->text();
    QString in_password = ui->lineedit_password->text();
    if (in_id.size() < 5 || in_id.size() > 12 || in_password.size() < 5 || in_password.size() > 12)
    {
        QMessageBox::information(this,
                                 tr("wrong"),
                                 tr("id和password的长度都要保持在5到12"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        return;
    }
    if (!ui->user_radio->isChecked())
    {
        QMessageBox::information(this,
                                 tr("wrong"),
                                 tr("注册请选择user"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        return;
    }
    if (id2password.count(in_id))
    {
        QMessageBox::information(this,
                                 tr("wrong register"),
                                 tr("此id已存在"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
    }
    else
    {
        id2password[in_id] = in_password;
        QString filename = in_id;
        id2password.insert({in_id, in_password});
        QFile file(QString("./data/users/") + filename);

        // 打开文件并检查是否成功
        if (file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out.setCodec("UTF-8"); // 确定编码格式
            //   写入数据到文件
            out << in_id << " " << in_password << "\n";
            out << QString("1 1 0\n");
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
            out << "\n0 0"; // 日常闹钟,临时闹钟
            // 关闭文件
            file.close();
        }
        else
        {
        }
        QMessageBox::information(this,
                                 tr("succeed register"),
                                 tr("注册成功"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        father->my_debugger->out("账号注册成功 id:" + in_id.toUtf8() + " password:" + in_password.toUtf8());
    }
}