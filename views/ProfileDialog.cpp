#include "ProfileDialog.h"
#include "controllers/AuthController.h"
#include "repositories/UserRepository.h"
#include <QMessageBox>
#include <QCryptographicHash>
#include <QRegularExpression>

ProfileDialog::ProfileDialog(AuthController* auth, QWidget *parent)
    : QDialog(parent), m_auth(auth)
{
    setWindowTitle("Изменение регистрационных данных");
    setModal(true);
    resize(400, 250);

    QVBoxLayout* layout = new QVBoxLayout(this);

    // Поля
    layout->addWidget(new QLabel("Текущий Email:"));
    m_oldEmailEdit = new QLineEdit(m_auth->currentUser().email());
    m_oldEmailEdit->setReadOnly(true);
    layout->addWidget(m_oldEmailEdit);

    layout->addWidget(new QLabel("Новый Email:"));
    m_newEmailEdit = new QLineEdit;
    layout->addWidget(m_newEmailEdit);

    layout->addWidget(new QLabel("Новый пароль:"));
    m_newPasswordEdit = new QLineEdit;
    m_newPasswordEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(m_newPasswordEdit);

    layout->addWidget(new QLabel("Подтверждение нового пароля:"));
    m_confirmPasswordEdit = new QLineEdit;
    m_confirmPasswordEdit->setEchoMode(QLineEdit::Password);
    layout->addWidget(m_confirmPasswordEdit);

    m_errorLabel = new QLabel;
    m_errorLabel->setStyleSheet("color: red;");
    layout->addWidget(m_errorLabel);

    // Кнопки
    QHBoxLayout* btnLayout = new QHBoxLayout;
    QPushButton* saveBtn = new QPushButton("Сохранить");
    QPushButton* cancelBtn = new QPushButton("Отмена");
    btnLayout->addWidget(saveBtn);
    btnLayout->addWidget(cancelBtn);
    layout->addLayout(btnLayout);

    connect(saveBtn, &QPushButton::clicked, this, &ProfileDialog::onSaveClicked);
    connect(cancelBtn, &QPushButton::clicked, this, &ProfileDialog::onCancelClicked);
}

void ProfileDialog::onSaveClicked()
{
    m_errorLabel->clear();

    QString newEmail = m_newEmailEdit->text().trimmed();
    QString newPassword = m_newPasswordEdit->text();
    QString confirmPassword = m_confirmPasswordEdit->text();

    if (!newEmail.isEmpty() && !newEmail.contains('@')) {
        m_errorLabel->setText("Введите корректный email.");
        return;
    }


    if (!newPassword.isEmpty()) {
        if (newPassword != confirmPassword) {
            m_errorLabel->setText("Пароли не совпадают.");
            return;
        }
        QRegularExpression passwordRegex(R"((?=.*\d)(?=.*[A-Z])(?=.*[!@#$%^&*()_+\-={}|;:,.<>?]).{8,})");
        if (!passwordRegex.match(newPassword).hasMatch()) {
            m_errorLabel->setText("Пароль должен быть ≥8 символов, содержать цифру, заглавную букву и спецсимвол.");
            return;
        }
    }

    UserRepository repo;
    User user = repo.findByEmail(m_auth->currentUser().email());
    if (user.id() == 0) {
        QMessageBox::warning(this, "Ошибка", "Пользователь не найден.");
        return;
    }

    if (!newEmail.isEmpty()) {
        User existing = repo.findByEmail(newEmail);
        if (existing.id() != 0 && existing.id() != user.id()) {
            m_errorLabel->setText("Этот email уже используется.");
            return;
        }
        user.setEmail(newEmail);
    }

    if (!newPassword.isEmpty()) {
        QString hash = QString::fromUtf8(
            QCryptographicHash::hash(newPassword.toUtf8(), QCryptographicHash::Sha256).toHex()
            );
        user.setPasswordHash(hash);
    }

    if (repo.update(user)) {
        m_auth->updateCurrentUser(user);
        QMessageBox::information(this, "Успех", "Данные обновлены.");
        accept();
    } else {
        QMessageBox::critical(this, "Ошибка", "Не удалось обновить данные.");
    }
}

void ProfileDialog::onCancelClicked()
{
    reject();
}