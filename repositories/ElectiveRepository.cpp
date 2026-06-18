#include "ElectiveRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

ElectiveRepository::ElectiveRepository()
{
}

QList<Elective> ElectiveRepository::executeSelectQuery(const QString& queryStr,
                                                       const QList<QVariant>& bindValues)
{
    QList<Elective> result;
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) return result;

    QSqlQuery query(db);
    if (!query.prepare(queryStr)) {
        qWarning() << "Ошибка подготовки:" << query.lastError().text();
        return result;
    }

    for (int i = 0; i < bindValues.size(); ++i)
        query.bindValue(i, bindValues.at(i));

    if (!query.exec()) {
        qWarning() << "Ошибка выполнения:" << query.lastError().text();
        return result;
    }

    while (query.next())
        result.append(Elective::fromRecord(query.record()));

    return result;
}

QList<Elective> ElectiveRepository::findAll()
{
    return executeSelectQuery("SELECT id, name, department_id FROM electives ORDER BY name");
}

QList<Elective> ElectiveRepository::findByDepartment(int departmentId)
{
    return executeSelectQuery("SELECT id, name, department_id FROM electives WHERE department_id = ?",
                              {departmentId});
}

Elective ElectiveRepository::findById(int id)
{
    QList<Elective> list = executeSelectQuery("SELECT id, name, department_id FROM electives WHERE id = ?", {id});
    return list.isEmpty() ? Elective() : list.first();
}

Elective ElectiveRepository::findByName(const QString& name)
{
    QList<Elective> list = executeSelectQuery("SELECT id, name, department_id FROM electives WHERE name = ?", {name});
    return list.isEmpty() ? Elective() : list.first();
}

int ElectiveRepository::insert(const Elective& elective)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) return -1;

    QSqlQuery query(db);
    query.prepare("INSERT INTO electives (name, department_id) VALUES (?, ?) RETURNING id");
    query.bindValue(0, elective.name());
    query.bindValue(1, elective.departmentId());

    if (!query.exec()) {
        qWarning() << "Ошибка вставки:" << query.lastError().text();
        return -1;
    }
    if (query.next()) return query.value(0).toInt();
    return -1;
}

bool ElectiveRepository::update(const Elective& elective)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen() || elective.id() == 0) return false;

    QSqlQuery query(db);
    query.prepare("UPDATE electives SET name=?, department_id=? WHERE id=?");
    query.bindValue(0, elective.name());
    query.bindValue(1, elective.departmentId());
    query.bindValue(2, elective.id());

    if (!query.exec()) {
        qWarning() << "Ошибка обновления:" << query.lastError().text();
        return false;
    }
    return true;
}

bool ElectiveRepository::remove(int id)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("DELETE FROM electives WHERE id = ?");
    query.bindValue(0, id);

    if (!query.exec()) {
        qWarning() << "Ошибка удаления:" << query.lastError().text();
        return false;
    }
    return true;
}
