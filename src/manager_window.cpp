#include "manager_window.h"

manager_window::manager_window(QWidget *parent)
    : QMainWindow(parent), father((class DataStructure_course_design *)parent), ui(new Ui_manager_window)
{
    ui->setupUi(this);
    this->setFixedSize(351, 286);
    connect(ui->add_button, SIGNAL(clicked()), this, SLOT(slot_click_button_add()));
    connect(ui->modify_button, SIGNAL(clicked()), this, SLOT(slot_click_button_modify()));
}

manager_window::~manager_window()
{
    delete ui;
}
void manager_window::slot_click_button_add()
{
    if (father->courses.count(ui->name->text()))
    {
        QMessageBox::information(this,
                                 tr("wrong"),
                                 tr("此课程已存在"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        return;
    }
    if (ui->day->text().toInt() <= 0 || ui->day->text().toInt() >= 8 || ui->time->text().toInt() <= 7 || ui->time->text().toInt() >= 18 || ui->num->text().toInt() <= 0 || ui->num->text().toInt() >= 4)
    {
        QMessageBox::information(this,
                                 tr("wrong"),
                                 tr("请输入正确的时间"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        return;
    }
    if (!father->my_navigation->name_to_node.count(ui->site->text()) || !father->my_navigation->name_to_node.count(ui->exam_site->text()))
    {
        QMessageBox::information(this,
                                 tr("wrong"),
                                 tr("请正确的地点"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        return;
    }
    course_information *tmp_ci = new course_information;
    tmp_ci->name = ui->name->text();
    for (int i = 1; i <= 18; i++)
    {
        tmp_ci->course_weeks.push_back(i);
    }
    tmp_ci->course_time.push_back({ui->day->text().toInt(), std::vector<int>(1, ui->time->text().toInt())});
    for (int i = 1; i < ui->num->text().toInt(); i++)
    {
        tmp_ci->course_time[0].second.push_back(ui->time->text().toInt() + i);
    }
    tmp_ci->exam_week = 18;
    tmp_ci->exam_time = tmp_ci->course_time[0];
    tmp_ci->course_site_building = ui->site->text();
    tmp_ci->course_site_room = ui->room->text();
    tmp_ci->exam_site_building = ui->exam_site->text();
    tmp_ci->exam_site_room = ui->exam_room->text();
    father->courses.insert({ui->name->text(), tmp_ci});
    QFile file(QString("./data/manager"));
    // 打开文件并检查是否成功
    if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
    {
        QTextStream fileOut(&file);
        fileOut.setCodec("UTF-8"); // 确定编码格式
        fileOut << "0\n";
        fileOut << ui->name->text() << " "
                << ui->day->text() << " "
                << ui->time->text() << " "
                << ui->num->text() << " "
                << ui->site->text() << " "
                << ui->room->text() << " "
                << ui->exam_site->text() << " "
                << ui->exam_room->text() << "\n";
        file.close();
        QMessageBox::information(this,
                                 tr("run"),
                                 tr("课程添加成功"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);

        father->my_select->init_combobox();
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
void manager_window::slot_click_button_modify()
{
    if (!father->courses.count(ui->name->text()))
    {
        QMessageBox::information(this,
                                 tr("wrong"),
                                 tr("此课程不存在"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        return;
    }
    if (ui->day->text().toInt() <= 0 || ui->day->text().toInt() >= 8 || ui->time->text().toInt() <= 7 || ui->time->text().toInt() >= 17 || ui->num->text().toInt() <= 0 || ui->num->text().toInt() >= 4)
    {
        QMessageBox::information(this,
                                 tr("wrong"),
                                 tr("请输入正确的时间"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        return;
    }
    if (!father->my_navigation->name_to_node.count(ui->site->text()) || !father->my_navigation->name_to_node.count(ui->exam_site->text()))
    {
        QMessageBox::information(this,
                                 tr("wrong"),
                                 tr("请正确的地点"),
                                 QMessageBox::Ok | QMessageBox::Cancel,
                                 QMessageBox::Ok);
        return;
    }
    QMessageBox::StandardButton reply;
    reply = QMessageBox::question(this, "课程修改", "修改存在的课程信息可能会造成用户日程表冲突, 所以\n如果想修改的话需要清空所有用户信息, 你确定要这样做吗?",
                                  QMessageBox::Yes | QMessageBox::No);
    if (reply == QMessageBox::Yes)
    {
        QString folderPath = "./data/users";
        QDir dir(folderPath);
        dir.setFilter(QDir::Files | QDir::NoDotAndDotDot | QDir::NoSymLinks);
        QFileInfoList fileList = dir.entryInfoList();
        foreach (QFileInfo file, fileList)
        {
            QFile currentFile(file.absoluteFilePath());
            if (!currentFile.remove())
            {
            }
        }
        father->my_login->id2password.clear();
        course_information *tmp_ci = father->courses[ui->name->text()];
        tmp_ci->course_weeks.clear();
        tmp_ci->course_time.clear();
        tmp_ci->name = ui->name->text();
        for (int i = 1; i <= 18; i++)
        {
            tmp_ci->course_weeks.push_back(i);
        }
        tmp_ci->course_time.push_back({ui->day->text().toInt(), std::vector<int>(1, ui->time->text().toInt())});
        for (int i = 1; i < ui->num->text().toInt(); i++)
        {
            tmp_ci->course_time[0].second.push_back(ui->time->text().toInt() + i);
        }
        tmp_ci->exam_week = 18;
        tmp_ci->exam_time = tmp_ci->course_time[0];
        tmp_ci->course_site_building = ui->site->text();
        tmp_ci->course_site_room = ui->room->text();
        tmp_ci->exam_site_building = ui->exam_site->text();
        tmp_ci->exam_site_room = ui->exam_room->text();
        QFile file(QString("./data/manager"));
        // 打开文件并检查是否成功
        if (file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::Append))
        {
            QTextStream fileOut(&file);
            fileOut.setCodec("UTF-8"); // 确定编码格式
            fileOut << "1\n";
            fileOut << ui->name->text() << " "
                    << ui->day->text() << " "
                    << ui->time->text() << " "
                    << ui->num->text() << " "
                    << ui->site->text() << " "
                    << ui->room->text() << " "
                    << ui->exam_site->text() << " "
                    << ui->exam_room->text() << "\n";
            file.close();
            QMessageBox::information(this,
                                     tr("run"),
                                     tr("课程修改成功"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
        }
        else
        {
            QMessageBox::information(this,
                                     tr("wrong"),
                                     tr("./data/manager打开失败"),
                                     QMessageBox::Ok | QMessageBox::Cancel,
                                     QMessageBox::Ok);
        }
        father->my_select->init_combobox();
    }
}