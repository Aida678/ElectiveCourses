#include "RegisterWindow.h"
#include "ui_registerwindow.h"
#include "controllers/AuthController.h"
#include <QMessageBox>

RegisterWindow::RegisterWindow(AuthController* controller, QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::RegisterWindow)
    , m_controller(controller)
{
    ui->setupUi(this);
    setWindowTitle("Регистрация нового пользователя");

    connect(ui->btnRegister, &QPushButton::clicked, this, &RegisterWindow::onRegisterButtonClicked);
    connect(ui->btnCancel, &QPushButton::clicked, this, &RegisterWindow::onCancelButtonClicked);
}

RegisterWindow::~RegisterWindow()
{
    delete ui;
}

void RegisterWindow::onRegisterButtonClicked()
{
    QString email = ui->lineEditEmail->text().trimmed();
    QString password = ui->lineEditPassword->text();
    QString confirm = ui->lineEditConfirmPassword->text();

    // Проверка совпадения паролей
    if (password != confirm) {
        QMessageBox::warning(this, "Ошибка", "Пароли не совпадают.");
        return;
    }

    // Вызываем метод контроллера
    QString error;
    if (m_controller->registerUser(email, password, error)) {
        QMessageBox::information(this, "Успех", "Регистрация прошла успешно. Теперь войдите в систему.");
        accept();   // закрываем диалог с кодом Accepted
    } else {
        QMessageBox::warning(this, "Ошибка", error);
    }
}

void RegisterWindow::onCancelButtonClicked()
{
    reject();   // закрываем диалог с кодом Rejected
}
