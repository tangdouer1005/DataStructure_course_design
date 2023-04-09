#include "DataStructure_course_design.h"

DataStructure_course_design::DataStructure_course_design(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_DataStructure_course_design)
{
    QApplication::setQuitOnLastWindowClosed(false);
    ui->setupUi(this);
    this->setFixedSize(1107, 637);

    member_init();
    read_course_information();
    get_course();

    my_login->show();
    my_debugger->show();
}

DataStructure_course_design::~DataStructure_course_design()
{
    delete ui;
}
void DataStructure_course_design::slot_click_cell(QTableWidgetItem *item)
{
    if (item->text() != QString(""))
    {
        (my_showevent->ui->show_something)->clear();
        auto this_course = courses[item->text()];
        // my_debugger->out(this_course -> );
        QString week = QString("上课周数: ");
        for (auto iter : this_course->course_weeks)
        {
            week += " ";
            week += QString::number(iter);
        }
        QString time = QString("上课时间: ");
        for (auto i : this_course->course_time)
        {
            int st = i.second[0];
            st += 7;
            int ed = st + i.second.size();
            time += QString("周") + QString::number(i.first) + " " + QString::number(st) + QString(":00~") + QString::number(ed) + QString(":00 ");
        }
        QString site = QString("上课地点: ") + this_course->course_site_building;
        QString final_exam_time = QString("期末时间: 第18周");
        // final_exam_time +=
        int st = (this_course->exam_time).second[0];
        st += 7;
        int ed = st + (this_course->exam_time).second.size();
        final_exam_time += QString::number(st) + QString(":00~") + QString::number(ed) + QString(":00 ");
        QString final_exam_site = "期末地点: " + this_course->exam_site_building;

        (my_showevent->ui->show_something)->append(item->text());
        (my_showevent->ui->show_something)->append(week);
        (my_showevent->ui->show_something)->append(time);
        (my_showevent->ui->show_something)->append(final_exam_time);
        (my_showevent->ui->show_something)->append(final_exam_site);
        my_showevent->show();
    }
}
void DataStructure_course_design::slot_login()
{
    QString in_id = my_login->ui->lineedit_id->text();
    QString in_password = my_login->ui->lineedit_password->text();

    if (my_login->id2password.count(in_id))
    {
        if (my_login->id2password[in_id] == in_password)
        {
            show();
            my_login->close();
            my_timer->start(TIME_UNIT);
        }
        else
        {
            QMessageBox::information(this,
                                     tr("wrong password"),
                                     tr("请输入正确的password"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
            my_debugger->out("wrong password");
        }
    }
    else
    {
        QMessageBox::information(this,
                                 tr("wrong id"),
                                 tr("请输入正确的id"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        my_debugger->out("no this id");
    }
}

void DataStructure_course_design::slot_timer_update()
{
    cur_time++;
    if (cur_time == 24)
    {
        cur_time = 0;
        cur_day++;
        if (cur_day == 8)
        {
            cur_day = 1;
            cur_week++;
            if (cur_week == 19)
            {
                // 本学期结束, 删掉窗口
            }
        }
    }
    ui->week_label->setText(QString::number(cur_week));
    ui->day_label->setText(QString::number(cur_day));
    ui->time_label->setText(QString::number(cur_time));
    my_debugger->out(QString::number(cur_week) + " " + QString::number(cur_day) + " " + QString::number(cur_time));
}

void DataStructure_course_design::slot_time_edit()
{
    cur_week = ui->week_spinbox->value();
    cur_day = ui->day_spinbox->value();
    cur_time = ui->time_spinbox->value();
    ui->week_label->setText(QString::number(cur_week));
    ui->day_label->setText(QString::number(cur_day));
    ui->time_label->setText(QString::number(cur_time));
}
void DataStructure_course_design::member_init()
{
    // 成员初始化, 内存分配
    my_debugger = new debug_label(this);
    my_navigation = new navigation_window(this);
    my_alarm = new alarm_window(this);
    my_login = new login_window(this);
    my_showevent = new showevent_window(this);
    my_timer = new QTimer(this);
    ui->my_schedule_table->setEditTriggers(QAbstractItemView::NoEditTriggers); // 课程表不可编辑设置

    ui->week_spinbox->setRange(1, 18);
    ui->day_spinbox->setRange(1, 7);
    ui->time_spinbox->setRange(0, 23);
    // ui->week_edit->setFont(QFont("黑体", 16));
    // ui->day_edit->setFont(QFont("黑体", 16));
    // ui->time_edit->setFont(QFont("黑体", 16));

    connect(ui->button_edit_time, SIGNAL(clicked()), this, SLOT(slot_time_edit()));
    connect(my_timer, SIGNAL(timeout()), this, SLOT(slot_timer_update()));

    connect((my_login->ui)->button_login, SIGNAL(clicked()), this, SLOT(slot_login()));
    connect(ui->button_navigation, SIGNAL(clicked()), my_navigation, SLOT(show()));
    connect(ui->button_alarmclock, SIGNAL(clicked()), my_alarm, SLOT(show()));
}
void DataStructure_course_design::read_course_information()
{
    // 课程信息读取
    QFile file_course("./data/course.txt");
    if (!file_course.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        my_debugger->out("文件打开失败");
    }
    QTextStream course_in(&file_course);
    course_in.setCodec("UTF-8"); // dd
    while (!course_in.atEnd())
    {
        course_information *tmp_ci = new course_information;
        course_in >> tmp_ci->name;
        int count_course;
        course_in >> count_course;
        while (count_course--)
        {
            int week;
            int time_begin;
            int time_last;
            course_in >> week >> time_begin >> time_last;
            std::vector<int> tmp_t;
            for (int i = 0; i < time_last; i++)
            {
                tmp_t.push_back(i + time_begin);
            }
            (tmp_ci->course_time).push_back({week, tmp_t});
        }
        int count_weeks;
        course_in >> count_weeks;
        if (count_weeks == 0)
        {
            for (int i = 1; i <= 17; i++)
            {
                (tmp_ci->course_weeks).push_back(i);
            }
        }
        else
        {
            int t_c;
            while (count_weeks--)
            {
                course_in >> t_c;
                (tmp_ci->course_weeks).push_back(t_c);
            }
        }
        course_in.readLine();
        tmp_ci->course_site_building = course_in.readLine();
        int exam_last;
        int exam_begin;
        course_in >> (tmp_ci->exam_time).first >> exam_begin >> exam_last;
        for (int i = 0; i < exam_last; i++)
        {
            (tmp_ci->exam_time).second.push_back(exam_begin + i);
        }
        course_in.readLine();
        tmp_ci->exam_site_building = course_in.readLine();
        courses.insert({tmp_ci->name, tmp_ci});
    }
}

void DataStructure_course_design::get_course()
{
    // 课程表初始化
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            // ui->my_schedule_table->setItem(i, j, new QTableWidgetItem(QString::number(i) + QString(" ") + QString::number(j)));
            ui->my_schedule_table->setItem(i, j, new QTableWidgetItem(""));
        }
    }
    connect(ui->my_schedule_table, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(slot_click_cell(QTableWidgetItem *)));
    // 课程读取, 并将其显示在课程表中
    QFile file_default_course("./data/default_course.txt");
    if (!file_default_course.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        my_debugger->out("文件打开失败");
    }
    QTextStream default_course_in(&file_default_course);
    default_course_in.setCodec("UTF-8");
    while (!default_course_in.atEnd())
    {
        QString cur_course_name = default_course_in.readLine();
        if (courses.count(cur_course_name))
        {
            for (auto i : (courses[cur_course_name])->course_time)
            {

                for (auto j : i.second)
                {
                    (ui->my_schedule_table->item(j + 1, i.first - 1))->setText(cur_course_name);
                }
            }
        }
    }
}
