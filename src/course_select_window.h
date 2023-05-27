#ifndef COURSE_SELECT_WINDOW_H
#define COURSE_SELECT_WINDOW_H
#include "ui_course_select_window.h"
#include <QMainWindow>
#include "DataStructure_course_design.h"
#include <set>

class DataStructure_course_design;
class course_select_window : public QMainWindow
{
    Q_OBJECT

public:
    course_select_window(QWidget *parent = nullptr);
    ~course_select_window();

    void closeEvent(QCloseEvent *event);

    void init_combobox();
    class DataStructure_course_design *father;
    std::set<QString> course_remain;

private:
    Ui_course_select_window *ui;
private slots:
    void slot_click_confirm();
};
#endif