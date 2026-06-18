#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QWidget>

namespace Ui { class LoginWindow; }

class AuthController;

class LoginWindow : public QWidget
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = nullptr);
    ~LoginWindow();

private slots:
    void onLoginClicked();
    void onRegisterClicked();

private:
    Ui::LoginWindow *ui;
    AuthController* m_controller;
};

#endif // LOGINWINDOW_H
