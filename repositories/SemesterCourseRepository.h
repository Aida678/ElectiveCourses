#ifndef SEMESTERCOURSEREPOSITORY_H
#define SEMESTERCOURSEREPOSITORY_H

#include <QList>
#include <QVariant>
#include "models/SemesterCourse.h"

class SemesterCourseRepository
{
public:
    SemesterCourseRepository();

    QList<SemesterCourse> findAll();
    QList<SemesterCourse> findByElective(int electiveId);
    QList<SemesterCourse> findBySemester(int semesterNumber);
    SemesterCourse findById(int id);
    int insert(const SemesterCourse& course);
    bool update(const SemesterCourse& course);
    bool remove(int id);

private:
    QList<SemesterCourse> executeSelectQuery(const QString& queryStr,
                                             const QList<QVariant>& bindValues = {});
};

#endif // SEMESTERCOURSEREPOSITORY_H
