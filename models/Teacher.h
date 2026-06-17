#ifndef TEACHER_H
#define TEACHER_H

#include <QString>
#include <QSqlRecord>

class Teacher
{
public:
    Teacher() = default;
    Teacher(int id, const QString& lastName, const QString& firstName,
            const QString& middleName, const QString& phone, int userId);

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    QString lastName() const { return m_lastName; }
    void setLastName(const QString& lastName) { m_lastName = lastName; }

    QString firstName() const { return m_firstName; }
    void setFirstName(const QString& firstName) { m_firstName = firstName; }

    QString middleName() const { return m_middleName; }
    void setMiddleName(const QString& middleName) { m_middleName = middleName; }

    QString phone() const { return m_phone; }
    void setPhone(const QString& phone) { m_phone = phone; }

    int userId() const { return m_userId; }
    void setUserId(int userId) { m_userId = userId; }

    static Teacher fromRecord(const QSqlRecord& record);

private:
    int m_id = 0;
    QString m_lastName;
    QString m_firstName;
    QString m_middleName;
    QString m_phone;
    int m_userId = 0;
};

#endif // TEACHER_H
