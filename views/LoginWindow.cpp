#include "LoginWindow.h"
#include "ui_loginwindow.h"
#include "controllers/AuthController.h"
#include "views/MainWindow.h"
#include "views/RegisterWindow.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::LoginWindow)
{
    ui->setupUi(this);
    m_controller = new AuthController(this);

    connect(ui->btnLogin, &QPushButton::clicked, this, &LoginWindow::onLoginClicked);
    connect(ui->btnRegister, &QPushButton::clicked, this, &LoginWindow::onRegisterClicked);
}

LoginWindow::~LoginWindow()
{
    delete ui;
}

void LoginWindow::onLoginClicked()
{
    QString email = ui->lineEditEmail->text().trimmed();
    QString password = ui->lineEditPassword->text();

    if (email.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "Ошибка", "Введите email и пароль.");
        return;
    }

    if (m_controller->login(email, password)) {
        MainWindow* main = new MainWindow(nullptr, m_controller, m_controller->currentUser());
        main->show();
        this->close();
    } else {
        QMessageBox::warning(this, "Ошибка", "Неверный email или пароль.");
    }
}


void LoginWindow::onRegisterClicked()
{
    RegisterWindow regWindow(m_controller, this);
    regWindow.exec();
}
