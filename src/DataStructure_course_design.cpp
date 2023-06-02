#include "DataStructure_course_design.h"

DataStructure_course_design::DataStructure_course_design(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_DataStructure_course_design)
{
    ui->setupUi(this);
    this->setFixedSize(1114, 699);

    member_init();
    read_course_information();

    my_login->show();
    my_debugger->show();
}

DataStructure_course_design::~DataStructure_course_design()
{
    QFile file(QString("./data/users/") + user->id);
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
        fileOut << my_alarm->alarming_dairy_event.size() << "\n";
        for (auto iter : my_alarm->alarming_dairy_event)
        {
            fileOut << iter << " ";
        }

        fileOut << "\n"
                << my_alarm->alarming_temporary_event.size() << "\n";
        for (auto iter : my_alarm->alarming_temporary_event)
        {
            fileOut << iter << " ";
        }
        file.close();
    }
    QFile logfile(QString("./data/log/") + user->id);
    // 打开文件并检查是否成功
    if (logfile.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream fileOut(&logfile);
        fileOut.setCodec("UTF-8");                        // 确定编码格式
        QString text = my_debugger->label->toPlainText(); // 获取 QTextBrowser 控件中的文本
        fileOut << text;
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
            if (item->text() == QString("temporary"))
            {
                (my_showevent->ui->show_something)->append("临时事务");
                my_debugger->out("临时事务查询");
            }
            else
            {
                for (auto iter : user->dairy_event)
                {
                    if (item->text() == iter.name)
                    {
                        (my_showevent->ui->show_something)->append("日常活动 " + iter.name);
                        if (iter.day == 0)
                        {
                            (my_showevent->ui->show_something)->append("时间:每天" + QString::number(iter.hour) + ":00");
                        }
                        else if (iter.week == 0)
                        {
                            (my_showevent->ui->show_something)->append("时间:每周周" + QString::number(iter.day) + " " + QString::number(iter.hour) + ":00");
                        }
                        else
                        {
                            (my_showevent->ui->show_something)->append("时间:第" + QString::number(iter.week) + "周周" + QString::number(iter.day) + " " + QString::number(iter.hour) + ":00");
                        }
                        (my_showevent->ui->show_something)->append("地点 " + iter.site);
                        my_debugger->out("日常活动查询");
                    }
                }
            }
        }
        else
        {
            auto this_course = courses[item->text()];
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
            my_debugger->out("课程查询" + item->text());
            my_navigation->clean_label();
            my_navigation->shortestPath(QString("学五公寓"), courses[item->text()]->course_site_building);
            my_navigation->ui->list_agenda->clear();
            my_navigation->ui->list_agenda->addItem(item->text());
            my_navigation->show();
        }
        my_showevent->show();
        stop();
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
    if (user->hour == 22)
    {
        QString c = "";
        std::set<QString> tmp;
        for (size_t i = 6; i <= 21; i++)
        {
            if (courses.count(schedule[user->week - 1][user->day % 7][i - 6]))
            {
                tmp.insert(schedule[user->week - 1][user->day % 7][i - 6]);
            }
        }
        if (tmp.size())
        {
            for (auto iter : tmp)
            {
                c += iter + "\n";
            }
            stop();
            QMessageBox::information(this,
                                     tr("明天的课程有"),
                                     tr(c.toUtf8()),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
            my_debugger->out("第二天课程提醒: week " + QString::number(user->week) + " day " + QString::number(user->day) + " time " + QString::number(user->day));
        }
    }
    if (user->hour >= 6 && user->hour <= 21)
    {
        if (my_alarm->alarming_dairy_event.count(schedule[user->week - 1][user->day - 1][user->hour - 6]))
        {
            QMessageBox::information(this,
                                     tr("日常活动闹钟"),
                                     tr(schedule[user->week - 1][user->day - 1][user->hour - 6].toUtf8()),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
            my_debugger->out("日常活动闹钟: week " + QString::number(user->week) + " day " + QString::number(user->day) + " time " + QString::number(user->day));
            QString site = "";
            for (auto iter : user->dairy_event)
            {
                if (iter.name == schedule[user->week - 1][user->day - 1][user->hour - 6])
                {
                    site = iter.site;
                }
            }
            if (site == QString(""))
            {
                QMessageBox::information(this,
                                         tr("没找到这个地点"),
                                         tr("rt"),
                                         QMessageBox::Ok | QMessageBox::Cancel,
                                         QMessageBox::Ok);
                return;
            }
            my_navigation->clean_label();
            my_navigation->shortestPath(QString("学五公寓"), site);
            my_navigation->ui->list_agenda->clear();
            my_navigation->ui->list_agenda->addItem(schedule[user->week - 1][user->day - 1][user->hour - 6]);
            stop();
            my_navigation->show();
        }
        if (schedule[user->week - 1][user->day - 1][user->hour - 6] == QString("temporary"))
        {
            QString some = QString("");
            auto tem = find_tem(user->week, user->day, user->hour);
            if (tem.size())
            {
                std::vector<QString> site;
                site.push_back(QString("学五公寓"));
                std::set<QString> hash;
                my_navigation->ui->list_agenda->clear();
                for (auto iter : tem)
                {
                    if (my_alarm->alarming_temporary_event.count(iter.name))
                    {
                        some += iter.name + " ";
                        hash.insert(iter.site);
                        my_navigation->ui->list_agenda->addItem(iter.site.toUtf8() + " " + iter.name.toUtf8());
                    }
                }
                for (auto iter : hash)
                {
                    site.push_back(iter);
                }
                my_navigation->find_shortest_path(site);
                stop();
                my_navigation->show();
                if (some != QString(""))
                {
                    QMessageBox::information(this,
                                             tr("临时事务闹钟"),
                                             tr(some.toUtf8()),
                                             QMessageBox::Ok | QMessageBox::Cancel,
                                             QMessageBox::Ok);
                    my_debugger->out("临时事务闹钟: week " + QString::number(user->week) + " day " + QString::number(user->day) + " time " + QString::number(user->day));
                }
            }
        }
        if (courses.count(schedule[user->week - 1][user->day - 1][user->hour - 6]))
        {
            if (user->hour - 6 - 1 >= 0 && schedule[user->week - 1][user->day - 1][user->hour - 6] == schedule[user->week - 1][user->day - 1][user->hour - 6 - 1])
            {
            }
            else
            {
                QMessageBox::information(this,
                                         tr("上课闹钟"),
                                         tr(schedule[user->week - 1][user->day - 1][user->hour - 6].toUtf8()),
                                         QMessageBox::Ok | QMessageBox::Cancel,
                                         QMessageBox::Ok);
                my_navigation->ui->list_agenda->clear();
                my_navigation->ui->list_agenda->addItem(schedule[user->week - 1][user->day - 1][user->hour - 6].toUtf8() + " " + courses[schedule[user->week - 1][user->day - 1][user->hour - 6]]->exam_site_building.toUtf8());
                my_navigation->clean_label();
                my_navigation->shortestPath(QString("学五公寓"), courses[schedule[user->week - 1][user->day - 1][user->hour - 6]]->course_site_building);
                stop();
                my_navigation->show();
                my_debugger->out("上课临近提醒闹钟: week " + QString::number(user->week) + " day " + QString::number(user->day) + " time " + QString::number(user->day));
            }
        }
    }

    ui->week_label->setText(QString::number(user->week));
    ui->day_label->setText(QString::number(user->day));
    ui->time_label->setText(QString::number(user->hour));
}

void DataStructure_course_design::slot_time_edit()
{
    user->week = ui->week_spinbox->value();
    user->day = ui->day_spinbox->value();
    user->hour = ui->time_spinbox->value();
    ui->week_label->setText(QString::number(user->week));
    ui->day_label->setText(QString::number(user->day));
    ui->time_label->setText(QString::number(user->hour));
    my_debugger->out("时间跳转:week" + QString::number(user->week) + " day " + QString::number(user->day) + " time " + QString::number(user->hour));

    set_schedule();
}
void DataStructure_course_design::slot_time_st()
{
    if (ui->button_time_st->text() == QString("停止"))
    {
        ui->button_time_st->setText("开始");
        my_debugger->out("时间暂停");
        my_timer->stop();
    }
    else
    {
        ui->button_time_st->setText("停止");
        my_debugger->out("时间开始");
        my_timer->start(TIME_UNIT);
    }
}

void DataStructure_course_design::init_log()
{
    QFile file_course("./data/log/" + user->id);
    if (!file_course.open(QIODevice::ReadOnly | QIODevice::Text))
    {
    }
    QTextStream course_in(&file_course);
    course_in.setCodec("UTF-8"); // 确定编码格式
    while (!course_in.atEnd())
    {
        my_debugger->out(course_in.readLine());
    }
}
void DataStructure_course_design::member_init()
{
    // 课程表初始化
    for (int i = 0; i < 16; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            ui->my_schedule_table->setItem(i, j, new QTableWidgetItem(""));
        }
    }
    // 成员初始化, 内存分配
    user = new user_information();
    user->week = 1, user->day = 1, user->hour = 0;
    my_debugger = new debug_label(this);
    my_navigation = new navigation_window(this);
    my_alarm = new alarm_window(this);
    my_login = new login_window(this);
    my_showevent = new showevent_window(this);
    my_add = new add_event_window(this);
    my_manager = new manager_window(this);
    my_select = new course_select_window(this);

    my_timer = new QTimer(this);
    ui->my_schedule_table->setEditTriggers(QAbstractItemView::NoEditTriggers); // 课程表不可编辑设置

    schedule = std::vector<std::vector<std::vector<QString>>>(18, std::vector<std::vector<QString>>(7 + 1, std::vector<QString>(15 + 1, QString(""))));

    ui->week_spinbox->setRange(1, 18);
    ui->day_spinbox->setRange(1, 7);
    ui->time_spinbox->setRange(0, 23);

    ui->week_spinbox_search->setRange(0, 18);
    ui->day_spinbox_search->setRange(0, 7);
    ui->time_spinbox_search->setRange(6, 22);

    connect(ui->button_edit_time, SIGNAL(clicked()), this, SLOT(slot_time_edit()));
    connect(ui->button_find, SIGNAL(clicked()), this, SLOT(slot_event_search()));
    connect(my_timer, SIGNAL(timeout()), this, SLOT(slot_timer_update()));
    connect(ui->button_navigation, &QPushButton::clicked, [=]()
            {my_navigation->show();stop(); });
    connect(ui->button_alarmclock, &QPushButton::clicked, [=]()
            {my_alarm->show();stop(); });
    connect(ui->button_time_st, SIGNAL(clicked()), this, SLOT(slot_time_st()));

    connect(ui->button_add, &QPushButton::clicked, [=]()
            {my_add->show();stop(); });
    connect(ui->my_schedule_table, SIGNAL(itemClicked(QTableWidgetItem *)), this, SLOT(slot_click_cell(QTableWidgetItem *)));
    // connect(my_add->ui->confirm_button, SIGNAL(clicked()), this, SLOT(slot_add_dairy()));
    QSize size = my_navigation->ui->label_map->sizeHint(); // 获取自适应大小后的控件大小
    int width = size.width();                              // 获取控件的宽度
    int height = size.height();                            // 获取控件的高度
}
void DataStructure_course_design::read_course_information()
{
    // 课程信息读取

    // 打开课程信息文件
    QFile file_course("./data/course.txt");
    if (!file_course.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::information(this,
                                 tr("wrong"),
                                 tr("./data/course.txt打开失败"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
    }
    QTextStream course_in(&file_course);
    course_in.setCodec("UTF-8"); // 确定编码格式

    while (!course_in.atEnd())
    {
        course_information *tmp_ci = new course_information;
        course_in >> tmp_ci->name;
        if (tmp_ci->name == QString(""))
            break;
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
        course_in >> tmp_ci->course_site_building;
        course_in >> tmp_ci->course_site_room;
        int exam_last;
        int exam_begin;
        course_in >> (tmp_ci->exam_time).first >> exam_begin >> exam_last;
        for (int i = 0; i < exam_last; i++)
        {
            (tmp_ci->exam_time).second.push_back(exam_begin + i);
        }
        course_in >> tmp_ci->exam_site_building;
        course_in >> tmp_ci->exam_site_room;
        courses.insert({tmp_ci->name, tmp_ci});
    }
    QFile file_man("./data/manager");
    if (file_man.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream file_man_in(&file_man);
        file_man_in.setCodec("UTF-8"); // 确定编码格式
        while (!file_man_in.atEnd())
        {
            QString type;
            QString name, day, time, num, site, room, exam_site, exam_room;
            file_man_in >> type;
            if (type == QString(""))
                continue;
            file_man_in >> name >> day >> time >> num >> site >> room >> exam_site >> exam_room;
            if (type == QString("0"))
            {
                course_information *tmp_ci = new course_information;
                tmp_ci->name = name;
                for (int i = 1; i <= 18; i++)
                {
                    tmp_ci->course_weeks.push_back(i);
                }
                tmp_ci->course_time.push_back({day.toInt(), std::vector<int>(1, time.toInt() - 5)});
                for (int i = 1; i < num.toInt(); i++)
                {
                    tmp_ci->course_time[0].second.push_back(time.toInt() - 5 + i);
                }
                tmp_ci->exam_week = 18;
                tmp_ci->exam_time = tmp_ci->course_time[0];
                tmp_ci->course_site_building = site;
                tmp_ci->course_site_room = room;
                tmp_ci->exam_site_building = exam_site;
                tmp_ci->exam_site_room = exam_room;
                courses.insert({name, tmp_ci});
            }
            else if (type == QString("1"))
            {
                course_information *tmp_ci = courses[name];
                tmp_ci->course_weeks.clear();
                tmp_ci->course_time.clear();
                tmp_ci->exam_time.second.clear();
                tmp_ci->name = name;
                for (int i = 1; i <= 18; i++)
                {
                    tmp_ci->course_weeks.push_back(i);
                }
                tmp_ci->course_time.push_back({day.toInt(), std::vector<int>(1, time.toInt() - 5)});
                for (int i = 1; i < num.toInt(); i++)
                {
                    tmp_ci->course_time[0].second.push_back(time.toInt() - 5 + i);
                }
                tmp_ci->exam_week = 18;
                tmp_ci->exam_time = tmp_ci->course_time[0];
                tmp_ci->course_site_building = site;
                tmp_ci->course_site_room = room;
                tmp_ci->exam_site_building = exam_site;
                tmp_ci->exam_site_room = exam_room;
            }
            else
            {
                QMessageBox::information(this,
                                         tr("wrong"),
                                         tr(type.toUtf8()),
                                         QMessageBox::Ok | QMessageBox::Cancel,
                                         QMessageBox::Ok);
            }
        }
    }
    else
    {
        QMessageBox::information(this,
                                 tr("wrong"),
                                 tr("./data/manager打开失败"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
    }
}

void DataStructure_course_design::get_course_event()
{
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

std::vector<event_information> DataStructure_course_design::find_tem(int week, int day, int time)
{
    std::vector<event_information> tem;
    for (auto iter : user->temporary_event)
    {
        if (iter.week == week && iter.day == day && iter.hour == time)
        {
            tem.push_back(iter);
        }
    }
    return tem;
}

void DataStructure_course_design::slot_event_search()
{
    if (!ui->dairy->isChecked() && !ui->tem->isChecked())
    {
        QMessageBox::information(this,
                                 tr("wrong"),
                                 tr("请选择一种事件"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        return;
    }
    int week = ui->week_spinbox_search->value();
    int day = ui->day_spinbox_search->value();
    int time = ui->time_spinbox_search->value();
    (my_showevent->ui->show_something)->clear();
    if (ui->dairy->isChecked())
    {
        sort(user->dairy_event.begin(), user->dairy_event.end());
        for (auto iter : user->dairy_event)
        {
            QString eve;
            eve += iter.name + " week:" + QString::number(iter.week);
            eve += " day:" + QString::number(iter.day);
            eve += " hour:" + QString::number(iter.hour);
            if (time == 0)
            {
                (my_showevent->ui->show_something)->append(eve);
            }
            else
            {
                if (time == 0)
                {
                    if (day == 0)
                    {
                        if (week == 0)
                        {
                            (my_showevent->ui->show_something)->append(eve);
                        }
                        else
                        {
                            if (iter.week == week)
                            {
                                (my_showevent->ui->show_something)->append(eve);
                            }
                        }
                    }
                    else
                    {
                        if (iter.week == week && iter.day == day)
                        {
                            (my_showevent->ui->show_something)->append(eve);
                        }
                    }
                }
                else
                {
                    if (iter.week == week && iter.day == day && iter.hour == time)
                    {
                        (my_showevent->ui->show_something)->append(eve);
                    }
                }
            }
        }
    }
    else if (ui->tem->isChecked())
    {
        for (auto iter : user->temporary_event)
        {
            QString eve;
            eve += iter.name + " week:" + QString::number(iter.week);
            eve += " day:" + QString::number(iter.day);
            eve += " hour:" + QString::number(iter.hour);
            if (time == 0)
            {
                (my_showevent->ui->show_something)->append(eve);
            }
            else
            {
                if (day == 0)
                {
                    if (iter.hour == time)
                    {
                        (my_showevent->ui->show_something)->append(eve);
                    }
                }
                else
                {
                    if (week == 0)
                    {
                        if (iter.hour == time && iter.day == day)
                        {
                            (my_showevent->ui->show_something)->append(eve);
                        }
                    }
                    else
                    {
                        if (iter.week == week && iter.day == day && iter.hour == time)
                        {
                            (my_showevent->ui->show_something)->append(eve);
                        }
                    }
                }
            }
        }
    }
    else
    {
    }
    stop();
    my_showevent->show();
}