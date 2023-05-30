#ifndef NAVIGATION_WINDOW_H
#define NAVIGATION_WINDOW_H
#include "DataStructure_course_design.h"
#include "ui_navigation_window.h"
#include <QMainWindow>
#include <queue>
#include <functional>
class DataStructure_course_design;
typedef struct node
{
    int x, y;
    QString name;
    node(int x, int y, QString name) : x(x), y(y), name(name) {}
    int distance(const node *other) const
    {
        if (x == other->x || y == other->y)
        {
            return abs(x - other->x) + abs(y - other->y);
        }
        return INT_MAX;
    }
} node;
class navigation_window : public QMainWindow
{
    Q_OBJECT

public:
    navigation_window(QWidget *parent = nullptr);
    ~navigation_window();
    std::map<QString, node *> name_to_node;
    std::multimap<int, node *> x_to_node;
    std::multimap<int, node *> y_to_node;
    QPainter *my_painter;
    QPixmap my_pic;
    class DataStructure_course_design *father;
    void init_node();
    void draw_line(int x1, int y1, int x2, int y2);
    void draw_point(int x, int y);
    void clean_label();

    void shortestPath(QString a, QString b);
    void find_shortest_path(const std::vector<QString> &building_names);
    Ui_navigation_window *ui;

private slots:
    void slot_click_button_navigation();
};
#endif