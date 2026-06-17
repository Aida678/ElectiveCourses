#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QObject>
#include "repositories/UserRepository.h"
#include "models/User.h"

class AuthController : public QObject
{
    Q_OBJECT

public:
    explicit AuthController(QObject *parent = nullptr);

    bool login(const QString& email, const QString& password);
    User currentUser() const { return m_user; }
    void logout();

private:
    UserRepository m_repo;
    User m_user;
};

#endif // AUTHCONTROLLER_H
