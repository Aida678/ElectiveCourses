#include "AuthController.h"
#include "repositories/UserRepository.h"
#include "repositories/TeacherRepository.h"
#include <QCryptographicHash>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

AuthController::AuthController(QObject *parent)
    : QObject(parent)
    , m_teacherId(0)
{
}

bool AuthController::login(const QString& email, const QString& password)
{
    UserRepository userRepo;
    if (!userRepo.checkPassword(email, password))
        return false;

    m_user = userRepo.findByEmail(email);
    if (m_user.id() == 0)
        return false;

    QSqlQuery query;
    query.prepare("SELECT r.name FROM roles r "
                  "JOIN user_roles ur ON ur.role_id = r.id "
                  "WHERE ur.user_id = :userId");
    query.bindValue(":userId", m_user.id());
    if (query.exec() && query.next()) {
        m_role = query.value(0).toString();
    } else {
        m_role = "student";
    }

    if (m_role == "teacher") {
        TeacherRepository teacherRepo;
        Teacher teacher = teacherRepo.findByUserId(m_user.id());
        if (teacher.id() != 0)
            m_teacherId = teacher.id();
        else
            m_teacherId = 0;
    } else {
        m_teacherId = 0;
    }

    return true;
}

void AuthController::logout()
{
    m_user = User();
    m_role.clear();
    m_teacherId = 0;
}

QString AuthController::roleDisplayName() const
{
    if (m_role == "admin") return "Администратор";
    if (m_role == "methodist") return "Методист";
    if (m_role == "teacher") return "Преподаватель";
    if (m_role == "student") return "Студент";
    return "Неизвестно";
}

bool AuthController::registerUser(const QString& email, const QString& password, QString& errorMessage)
{
    if (email.isEmpty() || password.isEmpty()) {
        errorMessage = "Email и пароль не могут быть пустыми.";
        return false;
    }
    if (password.length() < 6) {
        errorMessage = "Пароль должен содержать минимум 6 символов.";
        return false;
    }

    UserRepository repo;
    if (repo.findByEmail(email).id() != 0) {
        errorMessage = "Пользователь с таким email уже существует.";
        return false;
    }

    QString hash = QString::fromUtf8(
        QCryptographicHash::hash(password.toUtf8(), QCryptographicHash::Sha256).toHex()
        );
    User newUser(0, email, hash);
    int newId = repo.insert(newUser);
    if (newId > 0) {
        return true;
    } else {
        errorMessage = "Ошибка базы данных при создании пользователя.";
        return false;
    }
}

// ============= Права =============

bool AuthController::canManageUsers() const {
    return m_role == "admin";
}

bool AuthController::canManageProfile() const {
    return m_user.id() != 0;
}

bool AuthController::canCreateElective() const {
    return m_role == "admin" || m_role == "methodist";
}
bool AuthController::canReadElective() const {
    return m_role == "admin" || m_role == "methodist" || m_role == "teacher";
}
bool AuthController::canUpdateElective() const {
    return m_role == "admin" || m_role == "methodist";
}
bool AuthController::canDeleteElective() const {
    return m_role == "admin" || m_role == "methodist";
}

bool AuthController::canCreateStudent() const {
    return m_role == "admin" || m_role == "methodist";
}
bool AuthController::canReadStudent() const {
    return m_role == "admin" || m_role == "methodist" || m_role == "teacher";
}
bool AuthController::canUpdateStudent() const {
    return m_role == "admin" || m_role == "methodist";
}
bool AuthController::canDeleteStudent() const {
    return m_role == "admin" || m_role == "methodist";
}

bool AuthController::canCreateTeacher() const {
    return m_role == "admin" || m_role == "methodist";
}
bool AuthController::canReadTeacher() const {
    return m_role == "admin" || m_role == "methodist" || m_role == "teacher";
}
bool AuthController::canUpdateTeacher() const {
    return m_role == "admin" || m_role == "methodist";
}
bool AuthController::canDeleteTeacher() const {
    return m_role == "admin" || m_role == "methodist";
}

bool AuthController::canCreateSemesterCourse() const {
    return m_role == "admin" || m_role == "methodist";
}
bool AuthController::canReadSemesterCourse() const {
    return m_role == "admin" || m_role == "methodist" || m_role == "teacher";
}
bool AuthController::canUpdateSemesterCourse() const {
    return m_role == "admin" || m_role == "methodist";
}
bool AuthController::canDeleteSemesterCourse() const {
    return m_role == "admin" || m_role == "methodist";
}

bool AuthController::canCreateLesson() const {
    return m_role == "admin" || m_role == "methodist";
}
bool AuthController::canReadLesson() const {
    return m_role == "admin" || m_role == "methodist" || m_role == "teacher";
}
bool AuthController::canUpdateLesson() const {
    return m_role == "admin" || m_role == "methodist";
}
bool AuthController::canDeleteLesson() const {
    return m_role == "admin" || m_role == "methodist";
}

bool AuthController::canCreateTeacherCompetency() const {
    return m_role == "admin" || m_role == "methodist";
}
bool AuthController::canReadTeacherCompetency() const {
    return m_role == "admin" || m_role == "methodist" || m_role == "teacher";
}
bool AuthController::canUpdateTeacherCompetency() const {
    return m_role == "admin" || m_role == "methodist";
}
bool AuthController::canDeleteTeacherCompetency() const {
    return m_role == "admin" || m_role == "methodist";
}

bool AuthController::canCreateEnrollment() const {
    return m_role == "admin" || m_role == "methodist";
}
bool AuthController::canReadEnrollment() const {
    return m_role == "admin" || m_role == "methodist" || m_role == "teacher";
}
bool AuthController::canUpdateEnrollment() const {
    return m_role == "admin" || m_role == "methodist";
}
bool AuthController::canDeleteEnrollment() const {
    return m_role == "admin" || m_role == "methodist";
}

bool AuthController::canCreateExamSheet() const {
    return m_role == "admin" || m_role == "methodist" || m_role == "teacher";
}
bool AuthController::canReadExamSheet() const {
    return m_role == "admin" || m_role == "methodist" || m_role == "teacher";
}
bool AuthController::canUpdateExamSheet() const {
    return m_role == "admin" || m_role == "teacher";
}
bool AuthController::canDeleteExamSheet() const {
    return m_role == "admin";
}

bool AuthController::canCreateGrade() const {
    return canCreateExamSheet();
}
bool AuthController::canReadGrade() const {
    return canReadExamSheet();
}
bool AuthController::canUpdateGrade() const {
    return canUpdateExamSheet();
}
bool AuthController::canDeleteGrade() const {
    return canDeleteExamSheet();
}