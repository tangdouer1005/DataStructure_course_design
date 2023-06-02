#ifndef MANAGER_WINDOW_H
#define MANAGER_WINDOW_H
#include "ui_manager_window.h"
#include <QMainWindow>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QStringList>
#include <QDebug>

#include "DataStructure_course_design.h"
class DataStructure_course_design;

class manager_window : public QMainWindow
{
    Q_OBJECT

public:
    manager_window(QWidget *parent = nullptr);
    ~manager_window();

    Ui_manager_window *ui;
    class DataStructure_course_design *father;
private slots:
    void slot_click_button_add();
    void slot_click_button_modify();
};
#endif