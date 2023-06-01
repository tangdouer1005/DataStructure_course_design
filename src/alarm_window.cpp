#include "alarm_window.h"

alarm_window::alarm_window(QWidget *parent)
    : QMainWindow(parent), father((class DataStructure_course_design *)parent), ui(new Ui_alarm_window)
{
    ui->setupUi(this);
    this->setFixedSize(398, 375);
    connect(ui->add_dairy_button, SIGNAL(clicked()), this, SLOT(slot_click_dairy_button()));
    connect(ui->add_temporary_button, SIGNAL(clicked()), this, SLOT(slot_click_temporary_button()));
    connect(ui->show_alarm, SIGNAL(itemClicked(QListWidgetItem *)), this, SLOT(slot_click_list(QListWidgetItem *)));
    // connect(ui->show_alarm, &QListWidget::itemClicked, this, on_itemClicked);

    init_combo();
}

alarm_window::~alarm_window()
{
    delete ui;
}

void alarm_window::init_list()
{
    for (auto iname : alarming_temporary_event)
    {
        QString name = iname;
        class event_information *iter = NULL;
        for (size_t i = 0; i < father->user->temporary_event.size(); i++)
        {
            if (father->user->temporary_event[i].name == name)
            {
                iter = &father->user->temporary_event[i];
            }
        }
        if (!iter)
        {
            QMessageBox::information(this,
                                     tr("wrong"),
                                     tr("init_list tem"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
            return;
        }
        QString time = QString("第") + QString::number(iter->week) + QString("周周") + QString::number(iter->day) + QString(" ") + QString::number(iter->hour) + QString(":00");
        alarming_temporary_event.insert(name);
        QListWidgetItem *item = new QListWidgetItem("临时事务 " + name + QString(" ") + time);
        ui->show_alarm->addItem(item);
    }
    for (auto iname : alarming_dairy_event)
    {
        QString name = iname;
        QString time;
        class event_information *iter = NULL;
        for (size_t i = 0; i < father->user->dairy_event.size(); i++)
        {
            father->my_debugger->out(name);
            father->my_debugger->out(father->user->dairy_event[i].name);
            if (father->user->dairy_event[i].name == name)
            {
                iter = &father->user->dairy_event[i];
            }
        }
        if (!iter)
        {
            QMessageBox::information(this,
                                     tr("wrong"),
                                     tr("init_list dairy"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
            return;
        }
        if (iter->day == 0)
        {
            time = QString("每天") + QString::number(iter->hour) + QString(":00");
        }
        else if (iter->week == 0)
        {
            time = QString("每周周") + QString::number(iter->day) + QString(" ") + QString::number(iter->hour) + QString(":00");
        }
        else
        {
            time = QString("第") + QString::number(iter->week) + QString("周周") + QString::number(iter->day) + QString(" ") + QString::number(iter->hour) + QString(":00");
        }
        alarming_dairy_event.insert(name);
        QListWidgetItem *item = new QListWidgetItem(QString("日常活动 ") + name + QString(" ") + time);
        ui->show_alarm->addItem(item);
    }
    init_combo();
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
    class event_information *iter = NULL;
    for (size_t i = 0; i < father->user->temporary_event.size(); i++)
    {
        if (father->user->temporary_event[i].name == name)
        {
            iter = &father->user->temporary_event[i];
        }
    }
    QString time = QString("第") + QString::number(iter->week) + QString("周周") + QString::number(iter->day) + QString(" ") + QString::number(iter->hour) + QString(":00");
    alarming_temporary_event.insert(name);
    QListWidgetItem *item = new QListWidgetItem("临时事务 " + name + QString(" ") + time);
    ui->show_alarm->addItem(item);
    father->my_debugger->out("临时事务闹钟添加成功: name:" + name + QString(" ") + time);
    init_combo();
}
void alarm_window::slot_click_dairy_button()
{
    if (ui->dairy_event_combo->currentIndex() == -1)
    {
        return;
    }
    QString name = ui->dairy_event_combo->currentText();
    QString time;
    class event_information *iter = NULL;
    for (size_t i = 0; i < father->user->dairy_event.size(); i++)
    {
        if (father->user->dairy_event[i].name == name)
        {
            iter = &father->user->dairy_event[i];
        }
    }
    if (!iter)
    {
        QMessageBox::information(this,
                                 tr("wrong"),
                                 tr("in slot_click_temporary_button"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        return;
    }
    if (iter->day == 0)
    {
        time = QString("每天") + QString::number(iter->hour) + QString(":00");
    }
    else if (iter->week == 0)
    {
        time = QString("每周周") + QString::number(iter->day) + QString(" ") + QString::number(iter->hour) + QString(":00");
    }
    else
    {
        time = QString("第") + QString::number(iter->week) + QString("周周") + QString::number(iter->day) + QString(" ") + QString::number(iter->hour) + QString(":00");
    }
    alarming_dairy_event.insert(name);
    QListWidgetItem *item = new QListWidgetItem(QString("日常活动 ") + name + QString(" ") + time);
    ui->show_alarm->addItem(item);

    father->my_debugger->out("日常活动闹钟添加成功: name:" + name + QString(" ") + time);
    init_combo();
}
void alarm_window::slot_click_list(QListWidgetItem *content)
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "是否删除这个闹钟", "要删除吗?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QStringList parts = content->text().split(" ");
        if (parts[0] == QString("日常活动"))
        {
            father->my_debugger->out("日常活动闹钟删除: " + content->text());
            alarming_dairy_event.erase(parts[1]);
        }
        else
        {
            father->my_debugger->out("临时事务闹钟删除: " + content->text());
            alarming_temporary_event.erase(parts[1]);
        }
        delete content;
        init_combo();
    }
}