#ifndef STUDENTLEARNING_H
#define STUDENTLEARNING_H

#include <QString>
#include <QDate>
#include <QSqlRecord>

class StudentLearning
{
public:
    StudentLearning() = default;
    StudentLearning(int id, int grade, const QDate& examDate,
                    int teacherId, int semesterCourseId, int studentId);

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    int grade() const { return m_grade; }
    void setGrade(int grade) { m_grade = grade; }

    QDate examDate() const { return m_examDate; }
    void setExamDate(const QDate& examDate) { m_examDate = examDate; }

    int teacherId() const { return m_teacherId; }
    void setTeacherId(int teacherId) { m_teacherId = teacherId; }

    int semesterCourseId() const { return m_semesterCourseId; }
    void setSemesterCourseId(int semesterCourseId) { m_semesterCourseId = semesterCourseId; }

    int studentId() const { return m_studentId; }
    void setStudentId(int studentId) { m_studentId = studentId; }

    static StudentLearning fromRecord(const QSqlRecord& record);

private:
    int m_id = 0;
    int m_grade = 0;
    QDate m_examDate;
    int m_teacherId = 0;
    int m_semesterCourseId = 0;
    int m_studentId = 0;
};

#endif // STUDENTLEARNING_H
