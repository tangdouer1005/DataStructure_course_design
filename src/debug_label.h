#ifndef DEBUG_LABEL_H
#define DEBUG_LABEL_H

#include <QMainWindow>
#include <QTextBrowser>

class debug_label : public QMainWindow
{
    Q_OBJECT

public:
    QTextBrowser *label;
    debug_label(QWidget *parent = nullptr);
    void out(const QString &t);
    ~debug_label();
};

#endif