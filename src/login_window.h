#ifndef LOGIN_WINDOW_H
#define LOGIN_WINDOW_H

#include "ui_login_window.h"
#include "DataStructure_course_design.h"
#include <QMainWindow>
#include <QFile>
#include <QString>
#include <map>
#include <QTextStream>
#include <QMessageBox>
class DataStructure_course_design;
class login_window : public QMainWindow
{
    Q_OBJECT

public:
    login_window(QWidget *parent = nullptr);
    ~login_window();
    class DataStructure_course_design *father;

    std::map<QString, QString> id2password;
    Ui_login_window *ui;
private slots:
    void slot_login_click();
    void slot_register_click();
};
#endif