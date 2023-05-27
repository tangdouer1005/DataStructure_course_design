#include "DataStructure_course_design.h"

DataStructure_course_design::DataStructure_course_design(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_DataStructure_course_design)
{
    ui->setupUi(this);
    this->setFixedSize(1107, 637);

    member_init();
    read_course_information();

    my_login->show();
    my_debugger->show();
}

DataStructure_course_design::~DataStructure_course_design()
{
    QFile file(QString("./data/") + user->id);

    // 打开文件并检查是否成功
    if (file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream fileOut(&file);
        fileOut.setCodec("UTF-8"); // 确定编码格式

        fileOut << user->id << " " << user->password << "\n";
        fileOut << user->week << " " << user->day << " " << user->hour << "\n";

        fileOut << user->courses.size() << "\n";
        for (auto iter : user->courses)
        {
            fileOut << iter << QString(" ");
        }
        fileOut << "\n";

        fileOut << user->dairy_event.size() << "\n";
        for (auto iter : user->dairy_event)
        {
            fileOut << iter.name << " "
                    << iter.site << " "
                    << iter.week << " "
                    << iter.day << " "
                    << iter.hour << "\n";
        }

        fileOut << user->temporary_event.size() << "\n";
        for (auto iter : user->temporary_event)
        {
            fileOut << iter.name << " "
                    << iter.site << " "
                    << iter.week << " "
                    << iter.day << " "
                    << iter.hour << "\n";
        }
        file.close();
    }
    delete ui;
}
void DataStructure_course_design::slot_click_cell(QTableWidgetItem *item)
{
    if (item->text() != QString(""))
    {

        (my_showevent->ui->show_something)->clear();
        if (!courses.count(item->text()))
        {
            (my_showevent->ui->show_something)->append("这是一个日常事务");
        }
        else
        {
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
        }
        my_showevent->show();
    }
}

void DataStructure_course_design::slot_timer_update()
{
    user->hour++;
    if (user->hour == 24)
    {
        user->hour = 0;
        user->day++;
        if (user->day == 8)
        {
            user->day = 1;
            user->week++;
            set_schedule();
            if (user->week == 19)
            {
                exit(0);
                // 本学期结束, 删掉窗口
            }
        }
    }
    ui->week_label->setText(QString::number(user->week));
    ui->day_label->setText(QString::number(user->day));
    ui->time_label->setText(QString::number(user->hour));
    // my_debugger->out(QString::number(user->week) + " " + QString::number(user->day) + " " + QString::number(user->hour));
}

void DataStructure_course_design::slot_time_edit()
{
    user->week = ui->week_spinbox->value();
    user->day = ui->day_spinbox->value();
    user->hour = ui->time_spinbox->value();
    ui->week_label->setText(QString::number(user->week));
    ui->day_label->setText(QString::number(user->day));
    ui->time_label->setText(QString::number(user->hour));
    set_schedule();
}
void DataStructure_course_design::slot_time_st()
{
    if (ui->button_time_st->text() == QString("停止"))
    {
        ui->button_time_st->setText("开始");
        my_timer->stop();
    }
    else
    {
        ui->button_time_st->setText("停止");
        my_timer->start(TIME_UNIT);
    }
}

void DataStructure_course_design::member_init()
{
    // 成员初始化, 内存分配
    user = new user_information();
    user->week = 1, user->day = 1, user->hour = 0;
    my_debugger = new debug_label(this);
    my_navigation = new navigation_window(this);
    my_alarm = new alarm_window(this);
    my_login = new login_window(this);
    my_showevent = new showevent_window(this);
    my_add = new add_event_window(this);

    my_select = new course_select_window(this);

    my_timer = new QTimer(this);
    ui->my_schedule_table->setEditTriggers(QAbstractItemView::NoEditTriggers); // 课程表不可编辑设置

    schedule = std::vector<std::vector<std::vector<QString>>>(18, std::vector<std::vector<QString>>(7 + 1, std::vector<QString>(15 + 1, QString(""))));

    ui->week_spinbox->setRange(1, 18);
    ui->day_spinbox->setRange(1, 7);
    ui->time_spinbox->setRange(0, 23);

    connect(ui->button_edit_time, SIGNAL(clicked()), this, SLOT(slot_time_edit()));
    connect(my_timer, SIGNAL(timeout()), this, SLOT(slot_timer_update()));
    connect(ui->button_navigation, SIGNAL(clicked()), my_navigation, SLOT(show()));
    connect(ui->button_alarmclock, SIGNAL(clicked()), my_alarm, SLOT(show()));
    connect(ui->button_time_st, SIGNAL(clicked()), this, SLOT(slot_time_st()));

    connect(ui->button_add, SIGNAL(clicked()), my_add, SLOT(show()));
    // connect(my_add->ui->confirm_button, SIGNAL(clicked()), this, SLOT(slot_add_dairy()));
    QSize size = my_navigation->ui->label_map->sizeHint(); // 获取自适应大小后的控件大小
    int width = size.width();                              // 获取控件的宽度
    int height = size.height();                            // 获取控件的高度
    my_debugger->out(QString::number(width));
    my_debugger->out(QString::number(height));
}
void DataStructure_course_design::read_course_information()
{
    // 课程信息读取

    // 打开课程信息文件
    QFile file_course("./data/course.txt");
    if (!file_course.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        my_debugger->out("文件打开失败");
    }
    QTextStream course_in(&file_course);
    course_in.setCodec("UTF-8"); // 确定编码格式

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

void DataStructure_course_design::get_course_event()
{
    // 课程表初始化
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            ui->my_schedule_table->setItem(i, j, new QTableWidgetItem(""));
        }
    }

    connect(ui->my_schedule_table, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(slot_click_cell(QTableWidgetItem *)));
    for (auto course : user->courses)
    {
        if (courses.count(course))
        {
            for (auto week : (courses[course])->course_weeks)
            {
                for (auto day : (courses[course])->course_time)
                {
                    for (auto time : day.second)
                    {
                        schedule[week - 1][day.first - 1][time - 1] = course;
                    }
                }
            }
        }
    }
    for (auto iter : user->dairy_event)
    {
        if (iter.day == 0)
        {
            for (size_t w = 1; w <= 18; w++)
            {
                for (size_t d = 1; d <= 7; d++)
                {
                    schedule[w - 1][d - 1][iter.hour - 6] = iter.name;
                }
            }
        }
        else if (iter.week == 0)
        {
            for (size_t w = 1; w <= 18; w++)
            {
                schedule[w - 1][iter.day - 1][iter.hour - 6] = iter.name;
            }
        }
        else
        {
            schedule[iter.week - 1][iter.day - 1][iter.hour - 6] = iter.name;
        }
    }
    for (auto iter : user->temporary_event)
    {
        schedule[iter.week - 1][iter.day - 1][iter.hour - 6] = QString("temporary");
    }
    set_schedule();
}

void DataStructure_course_design::set_schedule()
{
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            (ui->my_schedule_table->item(j, i))->setText("");
        }
    }
    for (int i = 0; i < 7; i++)
    {
        for (int j = 0; j < 15; j++)
        {
            (ui->my_schedule_table->item(j, i))->setText(schedule[user->week - 1][i][j]);
        }
    }
}
