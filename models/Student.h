#ifndef STUDENT_H
#define STUDENT_H

#include <QString>
#include <QSqlRecord>

class Student
{
public:
    Student() = default;
    Student(int id, const QString& lastName, const QString& firstName,
            const QString& middleName, const QString& address, const QString& phone);

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    QString lastName() const { return m_lastName; }
    void setLastName(const QString& lastName) { m_lastName = lastName; }

    QString firstName() const { return m_firstName; }
    void setFirstName(const QString& firstName) { m_firstName = firstName; }

    QString middleName() const { return m_middleName; }
    void setMiddleName(const QString& middleName) { m_middleName = middleName; }

    QString address() const { return m_address; }
    void setAddress(const QString& address) { m_address = address; }

    QString phone() const { return m_phone; }
    void setPhone(const QString& phone) { m_phone = phone; }

    static Student fromRecord(const QSqlRecord& record);

private:
    int m_id = 0;
    QString m_lastName;
    QString m_firstName;
    QString m_middleName;
    QString m_address;
    QString m_phone;
};

#endif // STUDENT_H
