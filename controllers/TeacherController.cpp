#include "TeacherController.h"
#include "views/MainWindow.h"
#include "controllers/AuthController.h"
#include <QMessageBox>

TeacherController::TeacherController(MainWindow* view, AuthController* auth, QObject *parent)
    : QObject(parent), m_view(view), m_auth(auth)
{
    loadTeachers();
}

void TeacherController::loadTeachers()
{
    m_teachers = m_repo.findAll();
    m_view->updateTeacherTable(m_teachers);
    m_view->clearTeacherInputFields();
    m_view->showStatusMessage("Загружено " + QString::number(m_teachers.size()) + " преподавателей");
}

void TeacherController::addTeacher(const Teacher& teacher)
{
    if (!m_auth->canCreateTeacher()) {
        QMessageBox::warning(m_view, "Ошибка", "Недостаточно прав.");
        return;
    }
    if (teacher.lastName().isEmpty() || teacher.firstName().isEmpty() || teacher.phone().isEmpty()) {
        QMessageBox::warning(m_view, "Ошибка", "Фамилия, имя и телефон обязательны.");
        return;
    }

    int newId = m_repo.insert(teacher);
    if (newId > 0) {
        m_view->showStatusMessage("Преподаватель добавлен (ID: " + QString::number(newId) + ")");
        loadTeachers();
    } else {
        QMessageBox::critical(m_view, "Ошибка", "Не удалось добавить преподавателя.");
    }
}

void TeacherController::updateTeacher(const Teacher& teacher)
{
    if (!m_auth->canUpdateTeacher()) {
        QMessageBox::warning(m_view, "Ошибка", "Недостаточно прав.");
        return;
    }
    if (teacher.id() == 0) {
        QMessageBox::warning(m_view, "Ошибка", "Выберите преподавателя.");
        return;
    }

    if (m_repo.update(teacher)) {
        m_view->showStatusMessage("Преподаватель обновлён.");
        loadTeachers();
    } else {
        QMessageBox::critical(m_view, "Ошибка", "Не удалось обновить.");
    }
}

void TeacherController::deleteTeacher(int id)
{
    if (!m_auth->canDeleteTeacher()) {
        QMessageBox::warning(m_view, "Ошибка", "Недостаточно прав.");
        return;
    }
    if (id == 0) {
        QMessageBox::warning(m_view, "Ошибка", "Выберите преподавателя.");
        return;
    }

    if (QMessageBox::question(m_view, "Удаление", "Удалить преподавателя?") == QMessageBox::Yes) {
        if (m_repo.remove(id)) {
            m_view->showStatusMessage("Преподаватель удалён.");
            loadTeachers();
        } else {
            QMessageBox::critical(m_view, "Ошибка", "Не удалось удалить.");
        }
    }
}

void TeacherController::selectTeacher(int row)
{
    if (row < 0 || row >= m_teachers.size()) return;
    m_view->setTeacherInputFields(m_teachers[row]);
    m_view->showStatusMessage("Выбран: " + m_teachers[row].lastName() + " " + m_teachers[row].firstName());
}