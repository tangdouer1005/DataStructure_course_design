#include "course_select_window.h"

course_select_window::course_select_window(QWidget *parent)
    : QMainWindow(parent), father((class DataStructure_course_design *)parent), ui(new Ui_course_select_window)
{
    ui->setupUi(this);
    this->setFixedSize(434, 612);
    connect(ui->confirm_button, SIGNAL(clicked()), this, SLOT(slot_click_confirm()));
}

void course_select_window::init_combobox()
{
    for (auto iter : father->courses)
    {
        course_remain.insert(iter.first);
    }
    for (auto iter : father->user->courses)
    {
        if (course_remain.count(iter))
        {
            course_remain.erase(iter);
        }
    }
    for (auto iter : course_remain)
    {
        ui->courses_combobox->addItem(iter);
    }
}
void course_select_window::closeEvent(QCloseEvent *event)
{
    father->show();
    father->my_timer->start(TIME_UNIT);
}

course_select_window::~course_select_window()
{
    delete ui;
}
void course_select_window::slot_click_confirm()
{
    QString name = ui->courses_combobox->currentText();
    if (father->courses.count(name))
    {
        for (auto week : (father->courses[name])->course_weeks)
        {
            for (auto day : (father->courses[name])->course_time)
            {
                for (auto time : day.second)
                {
                    if (father->schedule[week - 1][day.first - 1][time + 1] == QString(""))
                    {
                    }
                    else
                    {
                        QMessageBox::information(this,
                                                 tr("与课程" + father->schedule[week - 1][day.first - 1][time + 1].toUtf8() + "冲突"),
                                                 tr("在第" + QString::number(week).toUtf8() + "周周" + QString::number(day.first).toUtf8() + QString::number(time).toUtf8() + ":00"),
                                                 QMessageBox::Ok | QMessageBox::Cancel,
                                                 QMessageBox::Ok);
                        return;
                    }
                }
            }
        }
        QMessageBox::information(this,
                                 tr("提示"),
                                 tr("添加成功"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        father->user->courses.push_back(name);
        father->get_course();
        father->set_schedule();
        course_remain.erase(name);
        // 处理schedule
    }
    else
    {
        QMessageBox::information(this,
                                 tr(name.toUtf8() + "不存在于father->courses"),
                                 tr("确认一下哪里有问题"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
    }
}