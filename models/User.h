#ifndef USER_H
#define USER_H

#include <QString>
#include <QSqlRecord>

class User
{
public:
    User() = default;
    User(int id, const QString& email, const QString& passwordHash);

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    QString email() const { return m_email; }
    void setEmail(const QString& email) { m_email = email; }

    QString passwordHash() const { return m_passwordHash; }
    void setPasswordHash(const QString& hash) { m_passwordHash = hash; }

    static User fromRecord(const QSqlRecord& record);

private:
    int m_id = 0;
    QString m_email;
    QString m_passwordHash;
};

#endif // USER_H
