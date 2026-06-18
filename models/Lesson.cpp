#include "Lesson.h"
#include <QVariant>

Lesson::Lesson(int id, int hoursCount, int teacherId, int lessonTypeId, int semesterCourseId)
    : m_id(id)
    , m_hoursCount(hoursCount)
    , m_teacherId(teacherId)
    , m_lessonTypeId(lessonTypeId)
    , m_semesterCourseId(semesterCourseId)
{
}

Lesson Lesson::fromRecord(const QSqlRecord& record)
{
    Lesson lesson;
    lesson.setId(record.value("id").toInt());
    lesson.setHoursCount(record.value("hours_count").toInt());
    lesson.setTeacherId(record.value("teacher_id").toInt());
    lesson.setLessonTypeId(record.value("lesson_type_id").toInt());
    lesson.setSemesterCourseId(record.value("semester_course_id").toInt());
    return lesson;
}
