#include "DataStructure_course_design.h"
#include <QApplication>
#pragma comment(lib, "user32.lib")
#include <QTextStream>
#include <QDebug>
int main(int argc, char *argv[])
{
    QApplication::setQuitOnLastWindowClosed(true);
    QApplication a(argc, argv);
    DataStructure_course_design w;

    // w.show();

    return a.exec();
}
// hyy first push5