#include "DataStructure_course_design.h"

#include <QApplication>
#pragma comment(lib, "user32.lib")

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    DataStructure_course_design w;
    w.show();
    return a.exec();
}