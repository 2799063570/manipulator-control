#include "widget.h"
#include "./ui_widget.h"

Widget::Widget(QWidget *parent) : QWidget(parent), ui(new Ui::Widget)
{
    ui->setupUi(this);
    robot_proxy_ = std::make_shared<RobotProxy>();
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_ip_edit_textEdited(const QString &arg1)
{
    ip_ = arg1;
}

void Widget::on_port_edit_textEdited(const QString &arg1)
{
    port_ = arg1.toInt();
}

void Widget::on_connect_btn_clicked()
{
    if (!ui->ip_edit->text().isEmpty()) {
        if (!ui->port_edit->text().isEmpty()) {
            robot_proxy_->connect(ip_, port_);
            robot_proxy_->login("aubo", "123456");
        } else {
            ui->port_edit->setStyleSheet(
                "QLineEdit{border:1px solid rgb(255, 0, 0);}");
            QString str = QString::fromLocal8Bit("Please input robot port!");
            ui->port_edit->setToolTip(
                QStringLiteral("Please input robot port!"));
        }
    } else {
        ui->ip_edit->setStyleSheet(
            "QLineEdit{border:1px solid rgb(255, 0, 0);}");
        QString str = QString::fromLocal8Bit("Please input robot ip!");
        ui->ip_edit->setToolTip(QStringLiteral("Please input robot ip!"));
    }
}

void Widget::on_disconnect_btn_clicked()
{
    robot_proxy_->disconnectFromServer();
}
