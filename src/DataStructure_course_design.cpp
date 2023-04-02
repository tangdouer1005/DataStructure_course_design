#include "DataStructure_course_design.h"

DataStructure_course_design::DataStructure_course_design(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_DataStructure_course_design)
{
    QApplication::setQuitOnLastWindowClosed(false);
    ui->setupUi(this);
    this->setFixedSize(1107, 637);

    my_debugger = new debug_label(this);
    my_navigation = new navigation_window(this);
    my_alarm = new alarm_window(this);
    my_login = new login_window(this);
    my_showevent = new showevent_window(this);

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

    // for (auto i : courses)
    // {
    //     my_debugger->out(i.first);
    //     my_debugger->out(QString::number(((i.second)->course_weeks).size()));
    //     my_debugger->out((i.second)->course_site_building);
    // }
    for (int i = 0; i < 15; i++)
    {
        for (int j = 0; j < 7; j++)
        {
            // ui->my_schedule_table->setItem(i, j, new QTableWidgetItem(QString::number(i) + QString(" ") + QString::number(j)));
            ui->my_schedule_table->setItem(i, j, new QTableWidgetItem(" "));
        }
    }
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
        // my_debugger->out(cur_course_name);
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

    connect(ui->button_navigation, SIGNAL(clicked()), my_navigation, SLOT(show()));
    connect(ui->button_alarmclock, SIGNAL(clicked()), my_alarm, SLOT(show()));
    // connect((my_login.ui)->button_login, SIGNAL(clicked()), this, SLOT(show()));

    my_login->show();
    connect((my_login->ui)->button_login, &QPushButton::clicked, [=]()
            {
                QString in_id = my_login->ui->lineedit_id->text();
                QString in_password = my_login->ui->lineedit_password->text();
                // my_debugger->out(in_password);
                // my_debugger->out(in_id);
                // int n = in_id.length();
                // my_debugger -> out(QString::number(n));
                if (my_login->id2password.count(in_id))
                {
                    if (my_login->id2password[in_id] == in_password)
                    {
                        show();
                        my_login->close();
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
                } });

    my_debugger->show();
    // for (auto iter : my_login->id2password)
    // {
    //     my_debugger->out(iter.first);
    //     int n = iter.first.length();
    //     my_debugger->out(QString::number(n));
    //     my_debugger->out(iter.second);
    // }
}

DataStructure_course_design::~DataStructure_course_design()
{
    delete ui;
}