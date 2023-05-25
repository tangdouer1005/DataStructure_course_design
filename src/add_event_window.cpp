#include "add_event_window.h"

add_event_window::add_event_window(QWidget *parent)
    : QMainWindow(parent), father((class DataStructure_course_design *)parent), ui(new Ui_add_event_window)
{
    ui->setupUi(this);
    this->setFixedSize(290, 339);

    ui->week_spinbox->setRange(1, 18);
    ui->day_spinbox->setRange(1, 7);
    ui->time_spinbox->setRange(6, 21);
    // connect(my_add->ui->confirm_button, SIGNAL(clicked()), this, SLOT(slot_add_dairy()));
    connect(ui->confirm_button, SIGNAL(clicked()), this, SLOT(slot_click_confirm_button()));
}

add_event_window::~add_event_window()
{
    delete ui;
}
void add_event_window::slot_click_confirm_button()
{
    int week = ui->week_spinbox->value();
    int day = ui->day_spinbox->value();
    int time = ui->time_spinbox->value();
    if (!ui->temporary_button->isChecked() && !ui->daily_button->isChecked())
    {
        QMessageBox::information(this,
                                 tr("提示"),
                                 tr("请选择一个事件类型"),
                                 QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
    }
    else if (ui->temporary_button->isChecked())
    {

        // 判断地点是否正确
        if (0)
        {
            QMessageBox::information(this,
                                     tr("错误"),
                                     tr("不存在此地点"),
                                     QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
        }
        if (father->schedule[week][day - 1][time - 6] == QString("") ||
            father->schedule[week][day - 1][time - 6] == QString("temporary"))
        {
            father->schedule[week][day - 1][time - 6] = QString("temporary"); // QString(ui->event_line->text());
            father->user->temporary_event.push_back({ui->event_line->text(),
                                                     ui->site_line->text(),
                                                     week, day, time});
            QMessageBox::information(this,
                                     tr("提示"),
                                     tr("添加成功"),
                                     QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
        }
        else
        {
            QMessageBox::information(this,
                                     tr("提示"),
                                     tr("该时间段已经被" + father->schedule[week][day - 1][time - 6].toUtf8() + "占用,推荐你选择其他时间"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
        }
    }
    else if (ui->daily_button->isChecked())
    {
        if (father->schedule[week][day - 1][time - 6] == QString(""))
        {
            father->schedule[week][day - 1][time - 6] = ui->event_line->text(); // QString(ui->event_line->text());
            father->user->dairy_event.push_back({ui->event_line->text(),
                                                 ui->site_line->text(),
                                                 week, day, time});
            QMessageBox::information(this,
                                     tr("提示"),
                                     tr("添加成功"),
                                     QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
        }
        else if (father->schedule[week][day - 1][time - 6] == QString("temporary"))
        {
            QMessageBox::StandardButton reply;
            reply = QMessageBox::question(this, "该时间段被临时事务占用", "你是否选择将其覆盖",
                                          QMessageBox::Yes | QMessageBox::No);
            if (reply == QMessageBox::Yes)
            {
                father->user->temporary_event.erase(std::remove_if(father->user->temporary_event.begin(),
                                                                   father->user->temporary_event.end(), [=](class event_information x)
                                                                   { 
                                                                        if(x.week == week && x.day == day && x.hour == time)
                                                                            return true;
                                                                        else
                                                                            return false; }),
                                                    father->user->temporary_event.end());
                father->schedule[week][day - 1][time - 6] = ui->event_line->text(); // QString(ui->event_line->text());
                father->user->dairy_event.push_back({ui->event_line->text(),
                                                     ui->site_line->text(),
                                                     week, day, time});
                QMessageBox::information(this,
                                         tr("提示"),
                                         tr("添加成功,原有的临时事务被删除"),
                                         QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
            }
            else
            {
                std::vector<int> goodtime;
                for (size_t i = 0; i < 16; i++)
                {
                    if (father->schedule[week][day - 1][i] == QString(""))
                    {
                        if (goodtime.size() == 3)
                        {
                            break;
                        }
                        else
                        {
                            goodtime.push_back(i + 6);
                        }
                    }
                }
                if (goodtime.size())
                {
                    QString time;
                    for (auto iter : goodtime)
                    {
                        time += QString::number(iter) + QString(":00~") + QString::number(iter + 1) + QString(":00 ");
                    }
                    QMessageBox::information(this,
                                             tr("剩余可行时间"),
                                             tr(time.toUtf8()),
                                             QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
                }
                else
                {
                    QMessageBox::information(this,
                                             tr("今天时间已满"),
                                             tr("添加失败"),
                                             QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
                }
            }
        }
        else
        {
            std::vector<int> goodtime;
            for (size_t i = 0; i < 16; i++)
            {
                if (father->schedule[week][day - 1][i] == QString(""))
                {
                    if (goodtime.size() == 3)
                    {
                        break;
                    }
                    else
                    {
                        goodtime.push_back(i + 6);
                    }
                }
            }
            if (goodtime.size())
            {
                QString time;
                for (auto iter : goodtime)
                {
                    time += QString::number(iter) + QString(":00~") + QString::number(iter + 1) + QString(":00 ");
                }
                QMessageBox::information(this,
                                         tr("剩余可行时间"),
                                         tr(time.toUtf8()),
                                         QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
            }
            else
            {
                QMessageBox::information(this,
                                         tr("今天时间已满"),
                                         tr("添加失败"),
                                         QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
            }
        }
    }
    else
    {
        // wrong
    }
    father->set_schedule();
}