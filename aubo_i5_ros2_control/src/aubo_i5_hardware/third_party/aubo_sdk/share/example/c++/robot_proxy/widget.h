#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "robot_proxy/robot_proxy.h"
using namespace arcs::aubo_sdk;

QT_BEGIN_NAMESPACE
namespace Ui {
class Widget;
}
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

private slots:

    void on_ip_edit_textEdited(const QString &arg1);

    void on_port_edit_textEdited(const QString &arg1);

    void on_connect_btn_clicked();

    void on_disconnect_btn_clicked();

private:
    Ui::Widget *ui;
    RobotProxyPtr robot_proxy_;
    QString ip_;
    int port_;
};
#endif // WIDGET_H
