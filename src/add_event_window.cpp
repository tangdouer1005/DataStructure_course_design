#include "add_event_window.h"

add_event_window::add_event_window(QWidget *parent)
    : QMainWindow(parent), father((class DataStructure_course_design *)parent), ui(new Ui_add_event_window)
{
    ui->setupUi(this);
    this->setFixedSize(290, 339);

    ui->week_spinbox->setRange(0, 18);
    ui->day_spinbox->setRange(0, 7);
    ui->time_spinbox->setRange(6, 21);
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
    // 判断地点是否正确
    if (!(father->my_navigation->name_to_node.count(ui->site_line->text())))
    {
        QMessageBox::information(this,
                                 tr("错误"),
                                 tr("不存在此地点"),
                                 QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
        return;
    }
    if (!ui->temporary_button->isChecked() && !ui->daily_button->isChecked())
    {
        QMessageBox::information(this,
                                 tr("提示"),
                                 tr("请选择一个事件类型"),
                                 QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
    }
    else if (ui->temporary_button->isChecked())
    {

        if (day == 0) // 每天都有
        {
            QMessageBox::information(this,
                                     tr("错误"),
                                     tr("临时事务不可设置每天都有"),
                                     QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
            return;
        }
        if (week == 0)
        {
            QMessageBox::information(this,
                                     tr("错误"),
                                     tr("临时事务不可设置每周都有"),
                                     QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
            return;
        }
        if (father->schedule[week - 1][day - 1][time - 6] == QString("") ||
            father->schedule[week - 1][day - 1][time - 6] == QString("temporary"))
        {
            father->schedule[week - 1][day - 1][time - 6] = QString("temporary");
            father->user->temporary_event.push_back({ui->event_line->text(),
                                                     ui->site_line->text(),
                                                     week, day, time});
            QMessageBox::information(this,
                                     tr("提示"),
                                     tr("添加成功"),
                                     QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
            father->my_debugger->out("临时事务添加成功: name:" + ui->event_line->text() + " site:" + ui->site_line->text() + " week:" + QString::number(week) + " day:" + QString::number(day) + " time:" + QString::number(time));
        }
        else
        {
            QMessageBox::information(this,
                                     tr("提示"),
                                     tr("该时间段已经被" + father->schedule[week - 1][day - 1][time - 6].toUtf8() + "占用,推荐你选择其他时间"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
        }
    }
    else if (ui->daily_button->isChecked())
    {
        if (day == 0)
        {
            for (size_t w = 1; w <= 18; w++)
            {
                for (size_t d = 1; d <= 7; d++)
                {
                    if (father->schedule[w - 1][d - 1][time - 6] != QString(""))
                    {
                        QMessageBox::information(this,
                                                 tr("提示"),
                                                 tr("该时间段已经被" + father->schedule[w - 1][d - 1][time - 6].toUtf8() + "占用,建议你选择其他时间"),
                                                 QMessageBox::Ok | QMessageBox::Cancel,
                                                 QMessageBox::Ok);
                        return;
                    }
                }
            }
            father->user->dairy_event.push_back({ui->event_line->text(),
                                                 ui->site_line->text(),
                                                 week, day, time});
            for (size_t w = 1; w <= 18; w++)
            {
                for (size_t d = 1; d <= 7; d++)
                {
                    father->schedule[w - 1][d - 1][time - 6] = ui->event_line->text();
                }
            }
            QMessageBox::information(this,
                                     tr("提示"),
                                     tr("事件添加成功"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
            father->my_debugger->out("日常活动添加成功: name:" + ui->event_line->text() + " site:" + ui->site_line->text() + " week:" + QString::number(week) + " day:" + QString::number(day) + " time:" + QString::number(time));
            father->set_schedule();
            father->my_alarm->init_combo();
            return;
        }
        if (week == 0)
        {
            for (size_t w = 1; w <= 18; w++)
            {
                if (father->schedule[w - 1][day - 1][time - 6] != QString(""))
                {
                    QMessageBox::information(this,
                                             tr("提示"),
                                             tr("该时间段已经被" + father->schedule[w - 1][day - 1][time - 6].toUtf8() + "占用,建议你选择其他时间"),
                                             QMessageBox::Ok | QMessageBox::Cancel,
                                             QMessageBox::Ok);
                    return;
                }
            }
            father->user->dairy_event.push_back({ui->event_line->text(),
                                                 ui->site_line->text(),
                                                 week, day, time});
            for (size_t w = 1; w <= 18; w++)
            {
                father->schedule[w - 1][day - 1][time - 6] = ui->event_line->text();
            }
            QMessageBox::information(this,
                                     tr("提示"),
                                     tr("事件添加成功"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
            father->my_debugger->out("日常活动添加成功: name:" + ui->event_line->text() + " site:" + ui->site_line->text() + " week:" + QString::number(week) + " day:" + QString::number(day) + " time:" + QString::number(time));
            father->set_schedule();
            father->my_alarm->init_combo();
            return;
        }
        if (father->schedule[week - 1][day - 1][time - 6] == QString(""))
        {
            father->schedule[week - 1][day - 1][time - 6] = ui->event_line->text();
            father->user->dairy_event.push_back({ui->event_line->text(),
                                                 ui->site_line->text(),
                                                 week, day, time});
            QMessageBox::information(this,
                                     tr("提示"),
                                     tr("添加成功"),
                                     QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
            father->my_debugger->out("日常活动添加成功: name:" + ui->event_line->text() + " site:" + ui->site_line->text() + " week:" + QString::number(week) + " day:" + QString::number(day) + " time:" + QString::number(time));
            father->set_schedule();
            father->my_alarm->init_combo();
        }
        else if (father->schedule[week - 1][day - 1][time - 6] == QString("temporary"))
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
                father->schedule[week - 1][day - 1][time - 6] = ui->event_line->text();
                father->user->dairy_event.push_back({ui->event_line->text(),
                                                     ui->site_line->text(),
                                                     week, day, time});
                father->set_schedule();
                father->my_alarm->init_combo();
                QMessageBox::information(this,
                                         tr("提示"),
                                         tr("添加成功,原有的临时事务被删除"),
                                         QMessageBox::Ok | QMessageBox::Cancel, QMessageBox::Ok);
                father->my_debugger->out("日常活动添加成功 同时此时的临时事务被覆盖: name:" + ui->event_line->text() + " site:" + ui->site_line->text() + " week:" + QString::number(week) + " day:" + QString::number(day) + " time:" + QString::number(time));
            }
            else
            {
                std::vector<int> goodtime;
                for (size_t i = 0; i < 16; i++)
                {
                    if (father->schedule[week - 1][day - 1][i] == QString(""))
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
                if (father->schedule[week - 1][day - 1][i] == QString(""))
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
    father->my_alarm->init_combo();
    father->set_schedule();
}