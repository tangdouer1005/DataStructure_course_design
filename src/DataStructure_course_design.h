#pragma once
#include "ui_DataStructure_course_design.h"
#include "debug_label.h"
#include <QMainWindow>
#include "navigation_window.h"
#include "alarm_window.h"
#include "login_window.h"
#include "showevent_window.h"
#include <QImage>
#include <QFile>
#include <QMessageBox>
#include <QPushButton>
#include <QString>
#include <QTextStream>
#include <QTextCodec>
#include <QTimer>
#include <string>
#include <vector>
#include <map>

#define TIME_UNIT 500
typedef struct course_information
{
    QString name;
    std::vector<int> course_weeks;
    std::vector<std::pair<int, std::vector<int>>> course_time;
    QString course_site_building;
    QString course_site_room;

    int exam_week;
    std::pair<int, std::vector<int>> exam_time;
    QString exam_site_building;
    QString exam_site_room;
} course_information;

class DataStructure_course_design : public QMainWindow
{
    Q_OBJECT

public:
    debug_label *my_debugger;
    navigation_window *my_navigation;
    alarm_window *my_alarm;
    login_window *my_login;
    showevent_window *my_showevent;

    QTimer *my_timer;

    int cur_week = 1;
    int cur_day = 1;
    int cur_time = 0;

    std::map<QString, course_information *> courses;

    DataStructure_course_design(QWidget *parent = nullptr);
    ~DataStructure_course_design();
    void member_init();
    void read_course_information();
    void get_course();

private slots:

    void slot_click_cell(QTableWidgetItem *);
    void slot_login();
    void slot_timer_update();

    void slot_time_edit();

private:
    Ui_DataStructure_course_design *ui;
};