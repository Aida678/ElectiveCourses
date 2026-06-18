#include "TeacherRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>

TeacherRepository::TeacherRepository()
{
}

QList<Teacher> TeacherRepository::executeSelectQuery(const QString& queryStr,
                                                     const QList<QVariant>& bindValues)
{
    QList<Teacher> result;
    QSqlDatabase db = DatabaseManager::instance().database();

    if (!db.isOpen()) {
        qWarning() << "База данных не открыта!";
        return result;
    }

    QSqlQuery query(db);
    if (!query.prepare(queryStr)) {
        qWarning() << "Ошибка подготовки запроса:" << query.lastError().text();
        return result;
    }

    for (int i = 0; i < bindValues.size(); ++i) {
        query.bindValue(i, bindValues.at(i));
    }

    if (!query.exec()) {
        qWarning() << "Ошибка выполнения запроса:" << query.lastError().text();
        return result;
    }

    while (query.next()) {
        QSqlRecord record = query.record();
        result.append(Teacher::fromRecord(record));
    }

    return result;
}

QList<Teacher> TeacherRepository::findAll()
{
    QString query = "SELECT id, last_name, first_name, middle_name, phone, user_id "
                    "FROM teachers ORDER BY last_name, first_name";
    return executeSelectQuery(query);
}

Teacher TeacherRepository::findById(int id)
{
    QString query = "SELECT id, last_name, first_name, middle_name, phone, user_id "
                    "FROM teachers WHERE id = ?";
    QList<Teacher> list = executeSelectQuery(query, {id});
    return list.isEmpty() ? Teacher() : list.first();
}

Teacher TeacherRepository::findByPhone(const QString& phone)
{
    QString query = "SELECT id, last_name, first_name, middle_name, phone, user_id "
                    "FROM teachers WHERE phone = ?";
    QList<Teacher> list = executeSelectQuery(query, {phone});
    return list.isEmpty() ? Teacher() : list.first();
}

Teacher TeacherRepository::findByUserId(int userId)
{
    QString query = "SELECT id, last_name, first_name, middle_name, phone, user_id "
                    "FROM teachers WHERE user_id = ?";
    QList<Teacher> list = executeSelectQuery(query, {userId});
    return list.isEmpty() ? Teacher() : list.first();
}

int TeacherRepository::insert(const Teacher& teacher)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) return -1;

    QSqlQuery query(db);
    query.prepare("INSERT INTO teachers (last_name, first_name, middle_name, phone, user_id) "
                  "VALUES (?, ?, ?, ?, ?) RETURNING id");

    query.bindValue(0, teacher.lastName());
    query.bindValue(1, teacher.firstName());
    query.bindValue(2, teacher.middleName());
    query.bindValue(3, teacher.phone());
    query.bindValue(4, teacher.userId());

    if (!query.exec()) {
        qWarning() << "Ошибка вставки преподавателя:" << query.lastError().text();
        return -1;
    }

    if (query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}

bool TeacherRepository::update(const Teacher& teacher)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen() || teacher.id() == 0) return false;

    QSqlQuery query(db);
    query.prepare("UPDATE teachers SET last_name=?, first_name=?, middle_name=?, "
                  "phone=?, user_id=? WHERE id=?");

    query.bindValue(0, teacher.lastName());
    query.bindValue(1, teacher.firstName());
    query.bindValue(2, teacher.middleName());
    query.bindValue(3, teacher.phone());
    query.bindValue(4, teacher.userId());
    query.bindValue(5, teacher.id());

    if (!query.exec()) {
        qWarning() << "Ошибка обновления преподавателя:" << query.lastError().text();
        return false;
    }
    return true;
}

bool TeacherRepository::remove(int id)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("DELETE FROM teachers WHERE id = ?");
    query.bindValue(0, id);

    if (!query.exec()) {
        qWarning() << "Ошибка удаления преподавателя:" << query.lastError().text();
        return false;
    }
    return true;
}
