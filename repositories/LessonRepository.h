#ifndef LESSONREPOSITORY_H
#define LESSONREPOSITORY_H

#include <QList>
#include <QVariant>
#include "models/Lesson.h"

class LessonRepository
{
public:
    LessonRepository();

    QList<Lesson> findAll();
    QList<Lesson> findByTeacher(int teacherId);
    QList<Lesson> findBySemesterCourse(int semesterCourseId);
    Lesson findById(int id);
    int insert(const Lesson& lesson);
    bool update(const Lesson& lesson);
    bool remove(int id);

private:
    QList<Lesson> executeSelectQuery(const QString& queryStr,
                                     const QList<QVariant>& bindValues = {});
};

#endif // LESSONREPOSITORY_H
