#include "User.h"
#include <QVariant>

User::User(int id, const QString& email, const QString& passwordHash)
    : m_id(id), m_email(email), m_passwordHash(passwordHash)
{
}

User User::fromRecord(const QSqlRecord& record)
{
    User user;
    user.setId(record.value("id").toInt());
    user.setEmail(record.value("email").toString());
    user.setPasswordHash(record.value("password_hash").toString());
    return user;
}
