#ifndef AUTHCONTROLLER_H
#define AUTHCONTROLLER_H

#include <QObject>
#include "models/User.h"

class AuthController : public QObject
{
    Q_OBJECT
public:
    explicit AuthController(QObject *parent = nullptr);

    bool login(const QString& email, const QString& password);
    void logout();
    User currentUser() const { return m_user; }

    QString currentRole() const { return m_role; }
    int currentTeacherId() const { return m_teacherId; }
    QString roleDisplayName() const;

    // Регистрация
    bool registerUser(const QString& email, const QString& password, QString& errorMessage);

    // === Права ===

    bool canManageUsers() const;
    bool canManageProfile() const;

    // Факультативы
    bool canCreateElective() const;
    bool canReadElective() const;
    bool canUpdateElective() const;
    bool canDeleteElective() const;

    // Студенты
    bool canCreateStudent() const;
    bool canReadStudent() const;
    bool canUpdateStudent() const;
    bool canDeleteStudent() const;

    // Преподаватели (используем те же права, что и для студентов)
    bool canCreateTeacher() const;
    bool canReadTeacher() const;
    bool canUpdateTeacher() const;
    bool canDeleteTeacher() const;

    // Семестровые курсы
    bool canCreateSemesterCourse() const;
    bool canReadSemesterCourse() const;
    bool canUpdateSemesterCourse() const;
    bool canDeleteSemesterCourse() const;

    // Нагрузка (Lessons)
    bool canCreateLesson() const;
    bool canReadLesson() const;
    bool canUpdateLesson() const;
    bool canDeleteLesson() const;

    // Компетенции преподавателей (TeacherElectives)
    bool canCreateTeacherCompetency() const;
    bool canReadTeacherCompetency() const;
    bool canUpdateTeacherCompetency() const;
    bool canDeleteTeacherCompetency() const;

    // Запись на факультатив (StudentElectives)
    bool canCreateEnrollment() const;
    bool canReadEnrollment() const;
    bool canUpdateEnrollment() const;
    bool canDeleteEnrollment() const;

    // Экзаменационные ведомости (StudentLearning)
    bool canCreateExamSheet() const;
    bool canReadExamSheet() const;
    bool canUpdateExamSheet() const;
    bool canDeleteExamSheet() const;

    // Оценки (используем те же права, что и для ведомостей)
    bool canCreateGrade() const;
    bool canReadGrade() const;
    bool canUpdateGrade() const;
    bool canDeleteGrade() const;

private:
    User m_user;
    QString m_role;
    int m_teacherId;
};

#endif // AUTHCONTROLLER_H