#include "Teacher.h"
#include <QVariant>

Teacher::Teacher(int id, const QString& lastName, const QString& firstName,
                 const QString& middleName, const QString& phone, int userId)
    : m_id(id)
    , m_lastName(lastName)
    , m_firstName(firstName)
    , m_middleName(middleName)
    , m_phone(phone)
    , m_userId(userId)
{
}

Teacher Teacher::fromRecord(const QSqlRecord& record)
{
    Teacher teacher;
    teacher.setId(record.value("id").toInt());
    teacher.setLastName(record.value("last_name").toString());
    teacher.setFirstName(record.value("first_name").toString());
    teacher.setMiddleName(record.value("middle_name").toString());
    teacher.setPhone(record.value("phone").toString());
    teacher.setUserId(record.value("user_id").toInt());
    return teacher;
}
