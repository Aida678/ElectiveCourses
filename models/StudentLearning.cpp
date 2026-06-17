#include "StudentLearning.h"

StudentLearning::StudentLearning(int id, int grade, const QDate& examDate,
                                 int teacherId, int semesterCourseId, int studentId)
    : m_id(id)
    , m_grade(grade)
    , m_examDate(examDate)
    , m_teacherId(teacherId)
    , m_semesterCourseId(semesterCourseId)
    , m_studentId(studentId)
{
}

StudentLearning StudentLearning::fromRecord(const QSqlRecord& record)
{
    StudentLearning learning;
    learning.setId(record.value("id").toInt());
    learning.setGrade(record.value("grade").toInt());
    learning.setExamDate(record.value("exam_date").toDate());
    learning.setTeacherId(record.value("teacher_id").toInt());
    learning.setSemesterCourseId(record.value("semester_course_id").toInt());
    learning.setStudentId(record.value("student_id").toInt());
    return learning;
}
