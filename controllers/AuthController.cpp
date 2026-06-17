#include "AuthController.h"

AuthController::AuthController(QObject *parent) : QObject(parent)
{
}

bool AuthController::login(const QString& email, const QString& password)
{
    if (m_repo.checkPassword(email, password)) {
        m_user = m_repo.findByEmail(email);
        return true;
    }
    return false;
}

void AuthController::logout()
{
    m_user = User();
}
