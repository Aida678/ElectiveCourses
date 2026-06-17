#include "SemesterCourse.h"

SemesterCourse::SemesterCourse(int id, int semesterNumber, const QString& academicYear, int electiveId)
    : m_id(id)
    , m_semesterNumber(semesterNumber)
    , m_academicYear(academicYear)
    , m_electiveId(electiveId)
{
}

SemesterCourse SemesterCourse::fromRecord(const QSqlRecord& record)
{
    SemesterCourse course;
    course.setId(record.value("id").toInt());
    course.setSemesterNumber(record.value("semester_number").toInt());
    course.setAcademicYear(record.value("academic_year").toString());
    course.setElectiveId(record.value("elective_id").toInt());
    return course;
}
