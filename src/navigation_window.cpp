#include "navigation_window.h"

navigation_window::navigation_window(QWidget *parent)
    : QMainWindow(parent), father((class DataStructure_course_design *)parent), ui(new Ui_navigation_window)
{
    ui->setupUi(this);
    my_painter = new QPainter();

    my_pic = QPixmap(ui->label_map->size());
    my_pic.fill(Qt::transparent);

    this->setFixedSize(1008, 763);
    init_node();
    connect(ui->button_navigation, SIGNAL(clicked()), this, SLOT(slot_click_button_navigation()));
}

navigation_window::~navigation_window()
{
    for (auto iter : name_to_node)
    {
        delete iter.second;
    }
    delete ui;
}

void navigation_window::slot_click_button_navigation()
{
    clean_label();
    father->my_debugger->out("导航查询:" + ui->start->text() + "到" + ui->end->text());
    shortestPath(ui->start->text(), ui->end->text());
}
void navigation_window::init_node()
{
    QFile file(QString("./data/map_coor.txt"));

    // 打开文件并检查是否成功
    if (file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QTextStream fileIn(&file);
        fileIn.setCodec("UTF-8"); // 确定编码格式
        while (!fileIn.atEnd())
        {
            QString name;
            int x, y;
            fileIn >> name >> x >> y;
            node *tmp = new node(x, y, name);
            name_to_node[name] = tmp;
        }

        file.close();
    }
    else
    {
        QMessageBox::information(this,
                                 tr("wrong"),
                                 tr("文件打开失败"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
    }
    for (auto iter : name_to_node)
    {
        x_to_node.insert({iter.second->x, iter.second});
        y_to_node.insert({iter.second->y, iter.second});
    }
}
#define ARROW_LEN 5
void navigation_window::draw_line(int x1, int y1, int x2, int y2)
{
    my_painter->begin(&my_pic);
    my_painter->setPen(Qt::black);
    my_painter->drawLine(x1, y1, x2, y2);
    my_painter->setPen(QPen(Qt::red, 3));
    if (x1 == x2)
    {
        if (y1 > y2)
        {
            my_painter->drawLine(x2, y2, x2 + ARROW_LEN, y2 + ARROW_LEN);
            my_painter->drawLine(x2, y2, x2 - ARROW_LEN, y2 + ARROW_LEN);
        }
        else
        {
            my_painter->drawLine(x2, y2, x2 - ARROW_LEN, y2 - ARROW_LEN);
            my_painter->drawLine(x2, y2, x2 + ARROW_LEN, y2 - ARROW_LEN);
        }
    }
    else
    {
        if (x1 > x2)
        {
            my_painter->drawLine(x2, y2, x2 + ARROW_LEN, y2 - ARROW_LEN);
            my_painter->drawLine(x2, y2, x2 + ARROW_LEN, y2 + ARROW_LEN);
        }
        else
        {
            my_painter->drawLine(x2, y2, x2 - ARROW_LEN, y2 - ARROW_LEN);
            my_painter->drawLine(x2, y2, x2 - ARROW_LEN, y2 + ARROW_LEN);
        }
    }
    my_painter->end();
    ui->label_map->setPixmap(my_pic);
}
void navigation_window::draw_point(int x, int y)
{
    my_painter->begin(&my_pic);
    my_painter->setPen(Qt::red);
    my_painter->drawPoint(x, y);
    my_painter->end();
    ui->label_map->setPixmap(my_pic);
}
void navigation_window::clean_label()
{
    my_pic.fill(Qt::transparent);
    ui->label_map->setPixmap(my_pic);
}
int xDistance(node *a, node *b)
{
    return abs(a->x - b->x);
}

// 获取节点之间的y距离
int yDistance(node *a, node *b)
{
    return abs(a->y - b->y);
}

// 判断两个节点是否相连
bool connected(node *a, node *b)
{
    return a->x == b->x || a->y == b->y;
}

void navigation_window::shortestPath(QString a, QString b)
{
    node *start, *end;
    if (name_to_node.count(a))
        start = name_to_node[a];
    else
    {
        QMessageBox::information(this,
                                 tr("没有"),
                                 tr(a.toUtf8()),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        return;
    }
    if (name_to_node.count(b))
        end = name_to_node[b];
    else
    {
        QMessageBox::information(this,
                                 tr("没有"),
                                 tr(b.toUtf8()),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        return;
    }
    using Pair = std::pair<int, QString>;
    std::priority_queue<Pair, std::vector<Pair>, std::greater<Pair>> q; // 路径长度，当前节点名称
    std::map<QString, QString> path;
    std::map<QString, int> path_length;

    q.push({0, start->name});
    path_length[start->name] = 0;

    while (!q.empty())
    {
        auto tmp = q.top();
        int length = tmp.first;
        auto current = tmp.second;
        q.pop();
        if (current == end->name)
        {
            break;
        }
        if (path_length[current] < length)
        {
            continue;
        }

        node *curNode = name_to_node[current];
        for (auto iter : x_to_node)
        {
            auto nextNode = iter.second;
            if (curNode == nextNode)
            {
                continue;
            }
            if (!connected(curNode, nextNode))
            {
                continue;
            }
            int newLength = length + xDistance(curNode, nextNode) + yDistance(curNode, nextNode);

            if (!path_length.count(nextNode->name) || path_length[nextNode->name] > newLength)
            {
                q.push({newLength, nextNode->name});
                path[nextNode->name] = current;
                path_length[nextNode->name] = newLength;
            }
        }
    }

    std::vector<QString> result;
    if (path_length.count(end->name))
    {
        QString cur = end->name;
        while (cur != start->name)
        {
            result.push_back(cur);
            cur = path[cur];
        }
        result.push_back(start->name);
        reverse(result.begin(), result.end());
    }

    std::pair<int, int> coor = {0, 0};
    for (size_t i = 0; i < result.size(); i++)
    {
        if (i == 0)
        {
            coor.first = name_to_node[result[i]]->x;
            coor.second = name_to_node[result[i]]->y;
        }
        else
        {
            draw_line(coor.first, coor.second, name_to_node[result[i]]->x, name_to_node[result[i]]->y);
            coor.first = name_to_node[result[i]]->x;
            coor.second = name_to_node[result[i]]->y;
        }
    }
}
int distance(node *n1, node *n2, std::multimap<int, node *> &x_to_node, std::multimap<int, node *> &y_to_node)
{
    if (n1->x == n2->x || n1->y == n2->y)
    {
        return n1->distance(n2);
    }
    int min_dist = INT_MAX;
    if (n1->x != n2->x)
    {
        auto x_range = x_to_node.equal_range(n1->x);
        for (auto it = x_range.first; it != x_range.second; ++it)
        {
            node *middle_node = it->second;
            int dist = n1->distance(middle_node) + middle_node->distance(n2);
            if (dist < min_dist)
            {
                min_dist = dist;
            }
        }
    }

    if (n1->y != n2->y)
    {
        auto y_range = y_to_node.equal_range(n1->y);
        for (auto it = y_range.first; it != y_range.second; ++it)
        {
            node *middle_node = it->second;
            int dist = n1->distance(middle_node) + middle_node->distance(n2);
            if (dist < min_dist)
            {
                min_dist = dist;
            }
        }
    }

    return min_dist;
}
void shortest_path_through_buildings(const std::vector<node *> &nodes, std::vector<node *> &current_path,
                                     std::vector<node *> &best_path, int &min_distance, int current_distance,
                                     std::multimap<int, node *> &x_to_node, std::multimap<int, node *> &y_to_node)
{
    if (current_path.size() == nodes.size())
    {
        current_distance += distance(current_path.back(), nodes[0], x_to_node, y_to_node);
        if (current_distance < min_distance)
        {
            min_distance = current_distance;
            best_path = current_path;
        }
        return;
    }

    for (node *n : nodes)
    {
        if (std::find(current_path.begin(), current_path.end(), n) != current_path.end())
        {
            continue;
        }
        current_path.push_back(n);
        int added_distance = current_path.size() == 1 ? 0 : distance(current_path[current_path.size() - 2], n, x_to_node, y_to_node);
        if (current_distance + added_distance >= min_distance)
        {
            current_path.pop_back();
            continue;
        }
        shortest_path_through_buildings(nodes, current_path, best_path, min_distance, current_distance + added_distance, x_to_node, y_to_node);
        current_path.pop_back();
    }
}
void navigation_window::find_shortest_path(const std::vector<QString> &building_names)
{
    clean_label();
    std::vector<node *> nodes;
    for (const auto &name : building_names)
    {
        nodes.push_back(name_to_node[name]);
    }

    std::vector<node *> current_path, best_path;
    int min_distance = INT_MAX;
    shortest_path_through_buildings(nodes, current_path, best_path, min_distance, 0, x_to_node, y_to_node);

    std::vector<QString> result;
    result.push_back(nodes[0]->name);
    for (const auto &n : best_path)
    {
        result.push_back(n->name);
    }
    result.push_back(nodes[0]->name);
    std::pair<int, int> coor = {0, 0};
    for (size_t i = 0; i + 1 < result.size(); i++)
    {
        shortestPath(result[i], result[i + 1]);
    }
}