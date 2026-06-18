#include "StudentRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QVariant>

StudentRepository::StudentRepository()
{
}

QList<Student> StudentRepository::executeSelectQuery(const QString& queryStr,
                                                     const QList<QVariant>& bindValues)
{
    QList<Student> result;
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
        result.append(Student::fromRecord(record));
    }

    return result;
}

QList<Student> StudentRepository::findAll()
{
    QString query = "SELECT id, last_name, first_name, middle_name, address, phone "
                    "FROM students ORDER BY last_name, first_name";
    return executeSelectQuery(query);
}

Student StudentRepository::findById(int id)
{
    QString query = "SELECT id, last_name, first_name, middle_name, address, phone "
                    "FROM students WHERE id = ?";
    QList<Student> list = executeSelectQuery(query, {id});
    return list.isEmpty() ? Student() : list.first();
}

Student StudentRepository::findByPhone(const QString& phone)
{
    QString query = "SELECT id, last_name, first_name, middle_name, address, phone "
                    "FROM students WHERE phone = ?";
    QList<Student> list = executeSelectQuery(query, {phone});
    return list.isEmpty() ? Student() : list.first();
}

int StudentRepository::insert(const Student& student)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) return -1;

    QSqlQuery query(db);
    query.prepare("INSERT INTO students (last_name, first_name, middle_name, address, phone) "
                  "VALUES (?, ?, ?, ?, ?) RETURNING id");

    query.bindValue(0, student.lastName());
    query.bindValue(1, student.firstName());
    query.bindValue(2, student.middleName());
    query.bindValue(3, student.address());
    query.bindValue(4, student.phone());

    if (!query.exec()) {
        qWarning() << "Ошибка вставки студента:" << query.lastError().text();
        return -1;
    }

    if (query.next()) {
        return query.value(0).toInt();
    }
    return -1;
}

bool StudentRepository::update(const Student& student)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen() || student.id() == 0) return false;

    QSqlQuery query(db);
    query.prepare("UPDATE students SET last_name=?, first_name=?, middle_name=?, "
                  "address=?, phone=? WHERE id=?");

    query.bindValue(0, student.lastName());
    query.bindValue(1, student.firstName());
    query.bindValue(2, student.middleName());
    query.bindValue(3, student.address());
    query.bindValue(4, student.phone());
    query.bindValue(5, student.id());

    if (!query.exec()) {
        qWarning() << "Ошибка обновления студента:" << query.lastError().text();
        return false;
    }
    return true;
}

bool StudentRepository::remove(int id)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("DELETE FROM students WHERE id = ?");
    query.bindValue(0, id);

    if (!query.exec()) {
        qWarning() << "Ошибка удаления студента:" << query.lastError().text();
        return false;
    }
    return true;
}
