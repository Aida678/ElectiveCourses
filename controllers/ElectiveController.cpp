#include "ElectiveController.h"
#include "views/MainWindow.h"
#include "controllers/AuthController.h"
#include <QMessageBox>

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