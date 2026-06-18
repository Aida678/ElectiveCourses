#ifndef TEACHERREPOSITORY_H
#define TEACHERREPOSITORY_H

#include <QList>
#include <QVariant>
#include "models/Teacher.h"

class TeacherRepository
{
public:
    TeacherRepository();

    QList<Teacher> findAll();
    Teacher findById(int id);
    Teacher findByPhone(const QString& phone);
    Teacher findByUserId(int userId);
    int insert(const Teacher& teacher);
    bool update(const Teacher& teacher);
    bool remove(int id);

private:
    QList<Teacher> executeSelectQuery(const QString& queryStr,
                                      const QList<QVariant>& bindValues = {});
};

#endif // TEACHERREPOSITORY_H
