#ifndef STUDENTREPOSITORY_H
#define STUDENTREPOSITORY_H

#include <QList>
#include <QVariant>
#include "models/Student.h"

class StudentRepository
{
public:
    StudentRepository();

    QList<Student> findAll();
    Student findById(int id);
    Student findByPhone(const QString& phone);
    int insert(const Student& student);
    bool update(const Student& student);
    bool remove(int id);

private:
    QList<Student> executeSelectQuery(const QString& queryStr,
                                      const QList<QVariant>& bindValues = {});
};

#endif // STUDENTREPOSITORY_H
