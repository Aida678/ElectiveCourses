#include "StudentController.h"
#include "views/MainWindow.h"
#include "controllers/AuthController.h"
#include <QMessageBox>

StudentController::StudentController(MainWindow* view, AuthController* auth, QObject *parent)
    : QObject(parent), m_view(view), m_auth(auth)
{
    loadStudents();
}

void StudentController::loadStudents()
{
    m_students = m_repo.findAll();
    m_view->updateStudentTable(m_students);
    m_view->clearStudentInputFields();
    m_view->showStatusMessage("Загружено " + QString::number(m_students.size()) + " студентов");
}

void StudentController::addStudent(const Student& student)
{
    if (!m_auth->canCreateStudent()) {
        QMessageBox::warning(m_view, "Ошибка", "Недостаточно прав.");
        return;
    }
    if (student.lastName().isEmpty() || student.firstName().isEmpty() || student.phone().isEmpty()) {
        QMessageBox::warning(m_view, "Ошибка", "Фамилия, имя и телефон обязательны.");
        return;
    }

    int newId = m_repo.insert(student);
    if (newId > 0) {
        m_view->showStatusMessage("Студент добавлен (ID: " + QString::number(newId) + ")");
        loadStudents();
    } else {
        QMessageBox::critical(m_view, "Ошибка", "Не удалось добавить студента. Проверьте телефон.");
    }
}

void StudentController::updateStudent(const Student& student)
{
    if (!m_auth->canUpdateStudent()) {
        QMessageBox::warning(m_view, "Ошибка", "Недостаточно прав.");
        return;
    }
    if (student.id() == 0) {
        QMessageBox::warning(m_view, "Ошибка", "Выберите студента.");
        return;
    }

    if (m_repo.update(student)) {
        m_view->showStatusMessage("Студент обновлён.");
        loadStudents();
    } else {
        QMessageBox::critical(m_view, "Ошибка", "Не удалось обновить.");
    }
}

void StudentController::deleteStudent(int id)
{
    if (!m_auth->canDeleteStudent()) {
        QMessageBox::warning(m_view, "Ошибка", "Недостаточно прав.");
        return;
    }
    if (id == 0) {
        QMessageBox::warning(m_view, "Ошибка", "Выберите студента.");
        return;
    }

    if (QMessageBox::question(m_view, "Удаление", "Удалить студента?") == QMessageBox::Yes) {
        if (m_repo.remove(id)) {
            m_view->showStatusMessage("Студент удалён.");
            loadStudents();
        } else {
            QMessageBox::critical(m_view, "Ошибка", "Не удалось удалить.");
        }
    }
}

void StudentController::selectStudent(int row)
{
    if (row < 0 || row >= m_students.size()) return;
    m_view->setStudentInputFields(m_students[row]);
    m_view->showStatusMessage("Выбран: " + m_students[row].lastName() + " " + m_students[row].firstName());
}