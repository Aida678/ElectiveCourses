#ifndef ELECTIVEREPOSITORY_H
#define ELECTIVEREPOSITORY_H

#include <QList>
#include <QVariant>
#include "models/Elective.h"

class ElectiveRepository
{
public:
    ElectiveRepository();

    QList<Elective> findAll();
    QList<Elective> findByDepartment(int departmentId);
    Elective findById(int id);
    Elective findByName(const QString& name);
    int insert(const Elective& elective);
    bool update(const Elective& elective);
    bool remove(int id);

private:
    QList<Elective> executeSelectQuery(const QString& queryStr,
                                       const QList<QVariant>& bindValues = {});
};

#endif // ELECTIVEREPOSITORY_H
