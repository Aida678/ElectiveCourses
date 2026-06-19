#include "ElectiveController.h"
#include "database/DatabaseManager.h"
#include "database/DatabaseManager.h"
#include "views/MainWindow.h"
#include "controllers/AuthController.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QSqlError>
#include <QVariant>
#include <QDebug>

ElectiveController::ElectiveController(MainWindow* view, AuthController* auth, QObject *parent)
    : QObject(parent), m_view(view), m_auth(auth)
{
    loadElectives();
}

void ElectiveController::loadElectives()
{
    m_electives = m_repo.findAll();
    m_view->updateElectiveTable(m_electives);
    m_view->clearElectiveInputFields();
    m_view->showStatusMessage("Загружено " + QString::number(m_electives.size()) + " факультативов");
}

void ElectiveController::addElective(const Elective& elective)
{
    if (!m_auth->canCreateElective()) {
        QMessageBox::warning(m_view, "Ошибка", "Недостаточно прав.");
        return;
    }
    if (elective.name().isEmpty() || elective.departmentId() == 0) {
        QMessageBox::warning(m_view, "Ошибка", "Название и кафедра обязательны.");
        return;
    }

    int newId = m_repo.insert(elective);
    if (newId > 0) {
        m_view->showStatusMessage("Факультатив добавлен (ID: " + QString::number(newId) + ")");
        loadElectives();
    } else {
        QMessageBox::critical(m_view, "Ошибка", "Не удалось добавить факультатив.");
    }
}

void ElectiveController::updateElective(const Elective& elective)
{
    if (!m_auth->canUpdateElective()) {
        QMessageBox::warning(m_view, "Ошибка", "Недостаточно прав.");
        return;
    }
    if (elective.id() == 0) {
        QMessageBox::warning(m_view, "Ошибка", "Выберите факультатив.");
        return;
    }

    if (m_repo.update(elective)) {
        m_view->showStatusMessage("Факультатив обновлён.");
        loadElectives();
    } else {
        QMessageBox::critical(m_view, "Ошибка", "Не удалось обновить.");
    }
}

void ElectiveController::deleteElective(int id)
{
    if (!m_auth->canDeleteElective()) {
        QMessageBox::warning(m_view, "Ошибка", "Недостаточно прав.");
        return;
    }
    if (id == 0) {
        QMessageBox::warning(m_view, "Ошибка", "Выберите факультатив.");
        return;
    }

    if (QMessageBox::question(m_view, "Удаление", "Удалить факультатив?") == QMessageBox::Yes) {
        if (m_repo.remove(id)) {
            m_view->showStatusMessage("Факультатив удалён.");
            loadElectives();
        } else {
            QMessageBox::critical(m_view, "Ошибка", "Не удалось удалить.");
        }
    }
}

void ElectiveController::selectElective(int row)
{
    if (row < 0 || row >= m_electives.size()) return;
    m_view->setElectiveInputFields(m_electives[row]);
    m_view->showStatusMessage("Выбран: " + m_electives[row].name());
}

QList<Elective> ElectiveController::getAllElectives() const
{
    return m_electives;
}

QList<StudentGrade> ElectiveController::getStudentGradesForElective(int electiveId)
{
    QList<StudentGrade> result;
    QSqlDatabase db = DatabaseManager::instance().database();
    if (!db.isOpen()) return result;

    QSqlQuery query(db);
    query.prepare(R"(
        SELECT s.id, s.last_name, s.first_name, s.middle_name, se.final_grade
        FROM students s
        JOIN student_electives se ON se.student_id = s.id
        WHERE se.elective_id = ?
        ORDER BY s.last_name, s.first_name
    )");
    query.bindValue(0, electiveId);

    if (!query.exec()) {
        qWarning() << "Ошибка загрузки итоговых оценок:" << query.lastError().text();
        return result;
    }

    while (query.next()) {
        StudentGrade grade(
            query.value("id").toInt(),
            query.value("last_name").toString(),
            query.value("first_name").toString(),
            query.value("middle_name").toString(),
            query.value("final_grade").toInt()
            );
        result.append(grade);
    }
    return result;
}