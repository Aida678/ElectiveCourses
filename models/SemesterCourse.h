#ifndef SEMESTERCOURSE_H
#define SEMESTERCOURSE_H

#include <QString>
#include <QSqlRecord>

class SemesterCourse
{
public:
    SemesterCourse() = default;
    SemesterCourse(int id, int semesterNumber, const QString& academicYear, int electiveId);

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    int semesterNumber() const { return m_semesterNumber; }
    void setSemesterNumber(int semesterNumber) { m_semesterNumber = semesterNumber; }

    QString academicYear() const { return m_academicYear; }
    void setAcademicYear(const QString& academicYear) { m_academicYear = academicYear; }

    int electiveId() const { return m_electiveId; }
    void setElectiveId(int electiveId) { m_electiveId = electiveId; }

    static SemesterCourse fromRecord(const QSqlRecord& record);

private:
    int m_id = 0;
    int m_semesterNumber = 0;
    QString m_academicYear;
    int m_electiveId = 0;
};

#endif // SEMESTERCOURSE_H
