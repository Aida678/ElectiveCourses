#include "SemesterCourseRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

SemesterCourseRepository::SemesterCourseRepository()
{
}

QList<SemesterCourse> SemesterCourseRepository::executeSelectQuery(const QString& queryStr,
                                                                   const QList<QVariant>& bindValues)
{
    QList<SemesterCourse> result;
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
        result.append(SemesterCourse::fromRecord(query.record()));

    return result;
}

QList<SemesterCourse> SemesterCourseRepository::findAll()
{
    return executeSelectQuery("SELECT id, semester_number, academic_year, elective_id FROM semester_courses ORDER BY academic_year, semester_number");
}

QList<SemesterCourse> SemesterCourseRepository::findByElective(int electiveId)
{
    return executeSelectQuery("SELECT id, semester_number, academic_year, elective_id FROM semester_courses WHERE elective_id = ?", {electiveId});
}

QList<SemesterCourse> SemesterCourseRepository::findBySemester(int semesterNumber)
{
    return executeSelectQuery("SELECT id, semester_number, academic_year, elective_id FROM semester_courses WHERE semester_number = ?", {semesterNumber});
}

SemesterCourse SemesterCourseRepository::findById(int id)
{
    QList<SemesterCourse> list = executeSelectQuery("SELECT id, semester_number, academic_year, elective_id FROM semester_courses WHERE id = ?", {id});
    return list.isEmpty() ? SemesterCourse() : list.first();
}

int SemesterCourseRepository::insert(const SemesterCourse& course)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) return -1;

    QSqlQuery query(db);
    query.prepare("INSERT INTO semester_courses (semester_number, academic_year, elective_id) VALUES (?, ?, ?) RETURNING id");
    query.bindValue(0, course.semesterNumber());
    query.bindValue(1, course.academicYear());
    query.bindValue(2, course.electiveId());

    if (!query.exec()) {
        qWarning() << "Ошибка вставки:" << query.lastError().text();
        return -1;
    }
    if (query.next()) return query.value(0).toInt();
    return -1;
}

bool SemesterCourseRepository::update(const SemesterCourse& course)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen() || course.id() == 0) return false;

    QSqlQuery query(db);
    query.prepare("UPDATE semester_courses SET semester_number=?, academic_year=?, elective_id=? WHERE id=?");
    query.bindValue(0, course.semesterNumber());
    query.bindValue(1, course.academicYear());
    query.bindValue(2, course.electiveId());
    query.bindValue(3, course.id());

    if (!query.exec()) {
        qWarning() << "Ошибка обновления:" << query.lastError().text();
        return false;
    }
    return true;
}

bool SemesterCourseRepository::remove(int id)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("DELETE FROM semester_courses WHERE id = ?");
    query.bindValue(0, id);

    if (!query.exec()) {
        qWarning() << "Ошибка удаления:" << query.lastError().text();
        return false;
    }
    return true;
}
