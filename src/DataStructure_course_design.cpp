#include "DataStructure_course_design.h"

DataStructure_course_design::DataStructure_course_design(QWidget *parent)
    : QMainWindow(parent), ui(new Ui_DataStructure_course_design)
{
    ui->setupUi(this);
    this->setFixedSize(1106, 556);
    // ui->hello_qt_world->setText("hello_qt_world");
    // ui->hello_qt_world->setText("hello_qt_world");

    my_debugger = new debug_label(this);
    my_navigation = new navigation_window(this);
    my_alarm = new alarm_window(this);
<<<<<<< HEAD
=======

>>>>>>> 035c40f7cc9a78447bbff8592f9b6805f19a8ff1
    my_login = new login_window(this);

    connect(ui->button_navigation, SIGNAL(clicked()), my_navigation, SLOT(show()));
    connect(ui->button_alarmclock, SIGNAL(clicked()), my_alarm, SLOT(show()));
    // connect((my_login.ui)->button_login, SIGNAL(clicked()), this, SLOT(show()));

    my_login->show();
    connect((my_login->ui)->button_login, &QPushButton::clicked, [=]()
            {show();my_login -> close(); });
    //  show();
    //   my_debugger->show();
    //   my_debugger->out("hello my_debugger!");
    //   my_debugger->out("you can do it! my_debugger!");
<<<<<<< HEAD
=======

>>>>>>> 035c40f7cc9a78447bbff8592f9b6805f19a8ff1
}

DataStructure_course_design::~DataStructure_course_design()
{
    delete ui;
}