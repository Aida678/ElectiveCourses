#ifndef LESSON_H
#define LESSON_H

#include <QSqlRecord>

class Lesson
{
public:
    Lesson() = default;
    Lesson(int id, int hoursCount, int teacherId, int lessonTypeId, int semesterCourseId);

    int id() const { return m_id; }
    void setId(int id) { m_id = id; }

    int hoursCount() const { return m_hoursCount; }
    void setHoursCount(int hoursCount) { m_hoursCount = hoursCount; }

    int teacherId() const { return m_teacherId; }
    void setTeacherId(int teacherId) { m_teacherId = teacherId; }

    int lessonTypeId() const { return m_lessonTypeId; }
    void setLessonTypeId(int lessonTypeId) { m_lessonTypeId = lessonTypeId; }

    int semesterCourseId() const { return m_semesterCourseId; }
    void setSemesterCourseId(int semesterCourseId) { m_semesterCourseId = semesterCourseId; }

    static Lesson fromRecord(const QSqlRecord& record);

private:
    int m_id = 0;
    int m_hoursCount = 0;
    int m_teacherId = 0;
    int m_lessonTypeId = 0;
    int m_semesterCourseId = 0;
};

#endif // LESSON_H
