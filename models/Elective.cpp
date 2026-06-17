#include "Elective.h"

Elective::Elective(int id, const QString& name, int departmentId)
    : m_id(id), m_name(name), m_departmentId(departmentId)
{
}

Elective Elective::fromRecord(const QSqlRecord& record)
{
    Elective elective;
    elective.setId(record.value("id").toInt());
    elective.setName(record.value("name").toString());
    elective.setDepartmentId(record.value("department_id").toInt());
    return elective;
}
