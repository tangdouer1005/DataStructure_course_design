#include "debug_label.h"

debug_label::debug_label(QWidget *parent)
    : QMainWindow(parent)
{
    label = new QTextBrowser(this);
    this->setFixedSize(800, 600);
    this->setWindowTitle("debuger_label");
    label->setFixedSize(800, 600);
    label->setFont(QFont(tr("Consolas"), 14));
}
void debug_label::out(const QString &t)
{
    label->append(t);
}
debug_label::~debug_label()
{
    delete label;
}
