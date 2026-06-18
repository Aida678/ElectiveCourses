#ifndef STUDENTLEARNINGREPOSITORY_H
#define STUDENTLEARNINGREPOSITORY_H

#include <QList>
#include <QVariant>
#include "models/StudentLearning.h"

class StudentLearningRepository
{
public:
    StudentLearningRepository();

    QList<StudentLearning> findAll();
    QList<StudentLearning> findByStudent(int studentId);
    QList<StudentLearning> findBySemesterCourse(int semesterCourseId);
    QList<StudentLearning> findByTeacher(int teacherId);
    StudentLearning findById(int id);
    int insert(const StudentLearning& learning);
    bool update(const StudentLearning& learning);
    bool remove(int id);

private:
    QList<StudentLearning> executeSelectQuery(const QString& queryStr,
                                              const QList<QVariant>& bindValues = {});
};

#endif // STUDENTLEARNINGREPOSITORY_H
