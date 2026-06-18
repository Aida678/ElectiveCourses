#include "LessonRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

LessonRepository::LessonRepository()
{
}

QList<Lesson> LessonRepository::executeSelectQuery(const QString& queryStr,
                                                   const QList<QVariant>& bindValues)
{
    QList<Lesson> result;
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
        result.append(Lesson::fromRecord(query.record()));

    return result;
}

QList<Lesson> LessonRepository::findAll()
{
    return executeSelectQuery("SELECT id, hours_count, teacher_id, lesson_type_id, semester_course_id FROM lessons");
}

QList<Lesson> LessonRepository::findByTeacher(int teacherId)
{
    return executeSelectQuery("SELECT id, hours_count, teacher_id, lesson_type_id, semester_course_id FROM lessons WHERE teacher_id = ?", {teacherId});
}

QList<Lesson> LessonRepository::findBySemesterCourse(int semesterCourseId)
{
    return executeSelectQuery("SELECT id, hours_count, teacher_id, lesson_type_id, semester_course_id FROM lessons WHERE semester_course_id = ?", {semesterCourseId});
}

Lesson LessonRepository::findById(int id)
{
    QList<Lesson> list = executeSelectQuery("SELECT id, hours_count, teacher_id, lesson_type_id, semester_course_id FROM lessons WHERE id = ?", {id});
    return list.isEmpty() ? Lesson() : list.first();
}

int LessonRepository::insert(const Lesson& lesson)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) return -1;

    QSqlQuery query(db);
    query.prepare("INSERT INTO lessons (hours_count, teacher_id, lesson_type_id, semester_course_id) "
                  "VALUES (?, ?, ?, ?) RETURNING id");
    query.bindValue(0, lesson.hoursCount());
    query.bindValue(1, lesson.teacherId());
    query.bindValue(2, lesson.lessonTypeId());
    query.bindValue(3, lesson.semesterCourseId());

    if (!query.exec()) {
        qWarning() << "Ошибка вставки:" << query.lastError().text();
        return -1;
    }
    if (query.next()) return query.value(0).toInt();
    return -1;
}

bool LessonRepository::update(const Lesson& lesson)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen() || lesson.id() == 0) return false;

    QSqlQuery query(db);
    query.prepare("UPDATE lessons SET hours_count=?, teacher_id=?, lesson_type_id=?, semester_course_id=? WHERE id=?");
    query.bindValue(0, lesson.hoursCount());
    query.bindValue(1, lesson.teacherId());
    query.bindValue(2, lesson.lessonTypeId());
    query.bindValue(3, lesson.semesterCourseId());
    query.bindValue(4, lesson.id());

    if (!query.exec()) {
        qWarning() << "Ошибка обновления:" << query.lastError().text();
        return false;
    }
    return true;
}

bool LessonRepository::remove(int id)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("DELETE FROM lessons WHERE id = ?");
    query.bindValue(0, id);

    if (!query.exec()) {
        qWarning() << "Ошибка удаления:" << query.lastError().text();
        return false;
    }
    return true;
}
