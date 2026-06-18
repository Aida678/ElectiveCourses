#include "LearningController.h"
#include "views/MainWindow.h"
#include <QMessageBox>

LearningController::LearningController(MainWindow* view, QObject *parent)
    : QObject(parent), m_view(view)
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
    if (learning.id() == 0) {
        QMessageBox::warning(m_view, "Ошибка", "Выберите запись.");
        return;
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
    if (id == 0) {
        QMessageBox::warning(m_view, "Ошибка", "Выберите запись.");
        return;
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
