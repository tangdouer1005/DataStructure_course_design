#ifndef DATASTRUCTURE_COUTSE_DESIGN_H
#define DATASTRUCTURE_COUTSE_DESIGN_H

#include "ui_DataStructure_course_design.h"
#include "debug_label.h"
#include <QMainWindow>
#include "navigation_window.h"
#include "alarm_window.h"
#include "login_window.h"
#include "showevent_window.h"
#include "add_event_window.h"
#include "course_select_window.h"
#include "manager_window.h"
#include <QFile>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QPainter>
#include <QTextStream>
#include <QTextCodec>
#include <QTimer>
#include <string>
#include <vector>
#include <map>
#include <algorithm>
#define TIME_UNIT 500
class navigation_window;
class course_select_window;
class login_window;
class add_event_window;
class alarm_window;
class manager_window;
typedef struct course_information
{
    // 该课程的名字
    QString name;
    // 有课程的周
    std::vector<int> course_weeks;
    // 在周几及其时间段
    std::vector<std::pair<int, std::vector<int>>> course_time;
    QString course_site_building;
    QString course_site_room;

    int exam_week;
    std::pair<int, std::vector<int>> exam_time;
    QString exam_site_building;
    QString exam_site_room;
} course_information;

enum EVENTTYPE
{
    DAILY,
    TEMPORARY
};
typedef struct event_information
{
    QString name;        // 活动名称
    QString site;        // 活动地点
    int week, day, hour; // 活动时间
    event_information(QString name, QString site, int week, int day, int hour) : name(name), site(site), week(week), day(day), hour(hour) {}
    bool operator<(const event_information &other) const
    {
        if (week < other.week)
        {
            return true;
        }
        else if (week > other.week)
        {
            return false;
        }
        else
        {
            if (day < other.day)
            {
                return true;
            }
            else if (day > other.day)
            {
                return false;
            }
            else
            {
                if (hour < other.hour)
                {
                    return true;
                }
                else
                {
                    return false;
                }
            }
        }
    }
} event_information;

typedef struct user_information
{
    QString id, password;
    int x, y;                                       // 当前坐标
    int week = 1, day = 1, hour = 0;                // 当前时间
    std::vector<QString> courses;                   // 所选课程
    std::vector<event_information> dairy_event;     // 日常课程储存
    std::vector<event_information> temporary_event; // 临时事务储存
} user_information;

class DataStructure_course_design : public QMainWindow
{
    Q_OBJECT

public:
    debug_label *my_debugger;
    navigation_window *my_navigation;
    alarm_window *my_alarm;
    login_window *my_login;
    showevent_window *my_showevent;
    add_event_window *my_add;
    course_select_window *my_select;
    manager_window *my_manager;
    QTimer *my_timer;

    user_information *user;

    std::map<QString, course_information *> courses;
    std::vector<std::vector<std::vector<QString>>> schedule;

    DataStructure_course_design(QWidget *parent = nullptr);
    ~DataStructure_course_design();
    void member_init();
    void init_log();
    void read_course_information();
    void get_course_event();
    void set_schedule();
    void stop()
    {
        my_timer->stop();
        ui->button_time_st->setText("开始");
    };
    std::vector<event_information> find_tem(int week, int day, int time);
    friend class add_event_window;
    friend class login_window;
    friend class course_select_window;
    friend class alarm_window;
    friend class manager_window;
private slots:

    void slot_click_cell(QTableWidgetItem *);
    void slot_timer_update();
    void slot_time_edit();
    void slot_time_st();
    void slot_event_search();

private:
    Ui_DataStructure_course_design *ui;
};
#endif