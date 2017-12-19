#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLayout>
#include <QList>
#include <QString>
#include <QPushButton>
#include <QLabel>
#include "flowlayout.h"

class LoginWindow:public QWidget
{
public:
    LoginWindow();
    LoginWindow(QString);

private:
    QLayout * layout;
    QVBoxLayout * loginLayout;
    QVBoxLayout * registLayout;
    QList<QWidget *> *loginWidgets;
    QList<QWidget *> *registWidgets;

    QPushButton * login;
    QPushButton * regist;
    QPushButton * s;
    QLabel * qlbName;
    QLabel * qlbPwd;

    void hideLogin();
    void showLogin();
    void hideRegist();
    void showRegist();
};

#endif // LOGINWINDOW_H
