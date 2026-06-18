#ifndef REGISTERWINDOW_H
#define REGISTERWINDOW_H

#include <QDialog>

namespace Ui { class RegisterWindow; }

class AuthController;

class RegisterWindow : public QDialog
{
    Q_OBJECT

public:
    explicit RegisterWindow(AuthController* controller, QWidget *parent = nullptr);
    ~RegisterWindow();

private slots:
    void onRegisterButtonClicked();
    void onCancelButtonClicked();

private:
    Ui::RegisterWindow *ui;
    AuthController* m_controller;
};

#endif // REGISTERWINDOW_H
