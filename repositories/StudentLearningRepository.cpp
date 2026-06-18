#include "StudentLearningRepository.h"
#include "database/DatabaseManager.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

StudentLearningRepository::StudentLearningRepository()
{
}

QList<StudentLearning> StudentLearningRepository::executeSelectQuery(const QString& queryStr,
                                                                     const QList<QVariant>& bindValues)
{
    QList<StudentLearning> result;
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
        result.append(StudentLearning::fromRecord(query.record()));

    return result;
}

QList<StudentLearning> StudentLearningRepository::findAll()
{
    return executeSelectQuery("SELECT id, grade, exam_date, teacher_id, semester_course_id, student_id FROM student_learning");
}

QList<StudentLearning> StudentLearningRepository::findByStudent(int studentId)
{
    return executeSelectQuery("SELECT id, grade, exam_date, teacher_id, semester_course_id, student_id FROM student_learning WHERE student_id = ?", {studentId});
}

QList<StudentLearning> StudentLearningRepository::findBySemesterCourse(int semesterCourseId)
{
    return executeSelectQuery("SELECT id, grade, exam_date, teacher_id, semester_course_id, student_id FROM student_learning WHERE semester_course_id = ?", {semesterCourseId});
}

QList<StudentLearning> StudentLearningRepository::findByTeacher(int teacherId)
{
    return executeSelectQuery("SELECT id, grade, exam_date, teacher_id, semester_course_id, student_id FROM student_learning WHERE teacher_id = ?", {teacherId});
}

StudentLearning StudentLearningRepository::findById(int id)
{
    QList<StudentLearning> list = executeSelectQuery("SELECT id, grade, exam_date, teacher_id, semester_course_id, student_id FROM student_learning WHERE id = ?", {id});
    return list.isEmpty() ? StudentLearning() : list.first();
}

int StudentLearningRepository::insert(const StudentLearning& learning)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) return -1;

    QSqlQuery query(db);
    query.prepare("INSERT INTO student_learning (grade, exam_date, teacher_id, semester_course_id, student_id) "
                  "VALUES (?, ?, ?, ?, ?) RETURNING id");
    query.bindValue(0, learning.grade());
    query.bindValue(1, learning.examDate());
    query.bindValue(2, learning.teacherId());
    query.bindValue(3, learning.semesterCourseId());
    query.bindValue(4, learning.studentId());

    if (!query.exec()) {
        qWarning() << "Ошибка вставки:" << query.lastError().text();
        return -1;
    }
    if (query.next()) return query.value(0).toInt();
    return -1;
}

bool StudentLearningRepository::update(const StudentLearning& learning)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen() || learning.id() == 0) return false;

    QSqlQuery query(db);
    query.prepare("UPDATE student_learning SET grade=?, exam_date=?, teacher_id=?, semester_course_id=?, student_id=? WHERE id=?");
    query.bindValue(0, learning.grade());
    query.bindValue(1, learning.examDate());
    query.bindValue(2, learning.teacherId());
    query.bindValue(3, learning.semesterCourseId());
    query.bindValue(4, learning.studentId());
    query.bindValue(5, learning.id());

    if (!query.exec()) {
        qWarning() << "Ошибка обновления:" << query.lastError().text();
        return false;
    }
    return true;
}

bool StudentLearningRepository::remove(int id)
{
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) return false;

    QSqlQuery query(db);
    query.prepare("DELETE FROM student_learning WHERE id = ?");
    query.bindValue(0, id);

    if (!query.exec()) {
        qWarning() << "Ошибка удаления:" << query.lastError().text();
        return false;
    }
    return true;
}
