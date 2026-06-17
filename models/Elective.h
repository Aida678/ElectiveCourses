#ifndef ELECTIVE_H
#define ELECTIVE_H

#include <QString>
#include <QSqlRecord>

class Elective
{
public:
    Elective() = default;
    Elective(int id, const QString& name, int departmentId);

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    QString name() const { return m_name; }
    void setName(const QString& name) { m_name = name; }

    int departmentId() const { return m_departmentId; }
    void setDepartmentId(int departmentId) { m_departmentId = departmentId; }

    static Elective fromRecord(const QSqlRecord& record);

private:
    int m_id = 0;
    QString m_name;
    int m_departmentId = 0;
};

#endif // ELECTIVE_H
