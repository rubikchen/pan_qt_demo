#include "LoginWindow.h"

LoginWindow::LoginWindow()
{
    layout = new FlowLayout;
    loginLayout = new QVBoxLayout;
    loginWidgets = new QList<QWidget *>;
    registLayout = new QVBoxLayout;
    registWidgets = new QList<QWidget *>;
}

LoginWindow::LoginWindow(QString)
{

}

void LoginWindow::hideLogin()
{

}

void LoginWindow::showLogin()
{

}

void LoginWindow::hideRegist()
{

}

void LoginWindow::showRegist()
{

}
