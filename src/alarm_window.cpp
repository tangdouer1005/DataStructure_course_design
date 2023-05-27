#include "alarm_window.h"

alarm_window::alarm_window(QWidget *parent)
    : QMainWindow(parent), father((class DataStructure_course_design *)parent), ui(new Ui_alarm_window)
{
    ui->setupUi(this);
    this->setFixedSize(398, 375);
    connect(ui->add_dairy_button, SIGNAL(clicked()), this, SLOT(slot_click_dairy_button()));
    connect(ui->add_temporary_button, SIGNAL(clicked()), this, SLOT(slot_click_temporary_button()));
    init_combo();
}

alarm_window::~alarm_window()
{
    delete ui;
}
void alarm_window::init_combo()
{
    ui->dairy_event_combo->clear();
    ui->temporary_event_combo->clear();
    for (auto iter : father->user->dairy_event)
    {
        ui->dairy_event_combo->addItem(iter.name);
    }

    for (auto iter : father->user->temporary_event)
    {
        ui->temporary_event_combo->addItem(iter.name);
    }

    for (auto iter : alarming_dairy_event)
    {
        int indexToRemove = ui->dairy_event_combo->findText(iter);
        if (indexToRemove != -1)
        {
            ui->dairy_event_combo->removeItem(indexToRemove);
        }
    }
    for (auto iter : alarming_temporary_event)
    {
        int indexToRemove = ui->temporary_event_combo->findText(iter);
        if (indexToRemove != -1)
        {
            ui->temporary_event_combo->removeItem(indexToRemove);
        }
    }
}
void alarm_window::slot_click_temporary_button()
{
    if (ui->temporary_event_combo->currentIndex() == -1)
    {
        return;
    }
    QString name = ui->temporary_event_combo->currentText();
    alarming_temporary_event.insert(name);
    QListWidgetItem *item = new QListWidgetItem(QString("临时事务") + name);
    ui->show_alarm->addItem(item);
    init_combo();
}
void alarm_window::slot_click_dairy_button()
{
    if (ui->dairy_event_combo->currentIndex() == -1)
    {
        return;
    }
    QString name = ui->dairy_event_combo->currentText();
    alarming_dairy_event.insert(name);
    QListWidgetItem *item = new QListWidgetItem(QString("日常活动") + name);
    ui->show_alarm->addItem(item);
    init_combo();
}
void alarm_window::slot_click_list(QListWidgetItem *)
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
    }
    else
}