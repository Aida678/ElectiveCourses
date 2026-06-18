#include "Student.h"
#include <QVariant>

Student::Student(int id, const QString& lastName, const QString& firstName,
                 const QString& middleName, const QString& address, const QString& phone)
    : m_id(id)
    , m_lastName(lastName)
    , m_firstName(firstName)
    , m_middleName(middleName)
    , m_address(address)
    , m_phone(phone)
{
}

Student Student::fromRecord(const QSqlRecord& record)
{
    Student student;
    student.setId(record.value("id").toInt());
    student.setLastName(record.value("last_name").toString());
    student.setFirstName(record.value("first_name").toString());
    student.setMiddleName(record.value("middle_name").toString());
    student.setAddress(record.value("address").toString());
    student.setPhone(record.value("phone").toString());
    return student;
}
