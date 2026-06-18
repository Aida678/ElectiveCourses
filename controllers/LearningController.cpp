#include "LearningController.h"
#include "views/MainWindow.h"
#include "controllers/AuthController.h"
#include <QMessageBox>

LearningController::LearningController(MainWindow* view, AuthController* auth, QObject *parent)
    : QObject(parent), m_view(view), m_auth(auth)
{
    loadLearnings();
}

void LearningController::loadLearnings()
{
    m_learnings = m_repo.findAll();
    m_view->updateLearningTable(m_learnings);
    m_view->clearLearningInputFields();
    m_view->showStatusMessage("Загружено " + QString::number(m_learnings.size()) + " записей об обучении");
}

void LearningController::addLearning(const StudentLearning& learning)
{
    if (!m_auth->canCreateExamSheet()) {
        QMessageBox::warning(m_view, "Ошибка", "Недостаточно прав.");
        return;
    }
    // Если преподаватель, проверяем, что teacher_id совпадает с его ID
    if (m_auth->currentRole() == "teacher") {
        if (learning.teacherId() != m_auth->currentTeacherId()) {
            QMessageBox::warning(m_view, "Ошибка", "Вы можете добавлять только свои ведомости.");
            return;
        }
    }
    if (learning.studentId() == 0 || learning.semesterCourseId() == 0) {
        QMessageBox::warning(m_view, "Ошибка", "Студент и семестровый курс обязательны.");
        return;
    }

    int newId = m_repo.insert(learning);
    if (newId > 0) {
        m_view->showStatusMessage("Запись добавлена (ID: " + QString::number(newId) + ")");
        loadLearnings();
    } else {
        QMessageBox::critical(m_view, "Ошибка", "Не удалось добавить запись.");
    }
}

void LearningController::updateLearning(const StudentLearning& learning)
{
    if (!m_auth->canUpdateExamSheet()) {
        QMessageBox::warning(m_view, "Ошибка", "Недостаточно прав.");
        return;
    }
    if (learning.id() == 0) {
        QMessageBox::warning(m_view, "Ошибка", "Выберите запись.");
        return;
    }
    // Если преподаватель, проверяем принадлежность
    if (m_auth->currentRole() == "teacher") {
        StudentLearning existing = m_repo.findById(learning.id());
        if (existing.teacherId() != m_auth->currentTeacherId()) {
            QMessageBox::warning(m_view, "Ошибка", "Вы можете редактировать только свои ведомости.");
            return;
        }
    }

    if (m_repo.update(learning)) {
        m_view->showStatusMessage("Запись обновлена.");
        loadLearnings();
    } else {
        QMessageBox::critical(m_view, "Ошибка", "Не удалось обновить.");
    }
}

void LearningController::deleteLearning(int id)
{
    if (!m_auth->canDeleteExamSheet()) {
        QMessageBox::warning(m_view, "Ошибка", "Недостаточно прав.");
        return;
    }
    if (id == 0) {
        QMessageBox::warning(m_view, "Ошибка", "Выберите запись.");
        return;
    }
    // Если преподаватель, проверяем принадлежность
    if (m_auth->currentRole() == "teacher") {
        StudentLearning existing = m_repo.findById(id);
        if (existing.teacherId() != m_auth->currentTeacherId()) {
            QMessageBox::warning(m_view, "Ошибка", "Вы можете удалять только свои ведомости.");
            return;
        }
    }

    if (QMessageBox::question(m_view, "Удаление", "Удалить запись?") == QMessageBox::Yes) {
        if (m_repo.remove(id)) {
            m_view->showStatusMessage("Запись удалена.");
            loadLearnings();
        } else {
            QMessageBox::critical(m_view, "Ошибка", "Не удалось удалить.");
        }
    }
}

void LearningController::selectLearning(int row)
{
    if (row < 0 || row >= m_learnings.size()) return;
    m_view->setLearningInputFields(m_learnings[row]);
    m_view->showStatusMessage("Выбрана запись ID: " + QString::number(m_learnings[row].id()));
}