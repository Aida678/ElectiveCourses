#ifndef LEARNINGCONTROLLER_H
#define LEARNINGCONTROLLER_H

#include <QObject>
#include "repositories/StudentLearningRepository.h"
#include "models/StudentLearning.h"

class MainWindow;
class AuthController;

class LearningController : public QObject
{
    Q_OBJECT

public:
    explicit LearningController(MainWindow* view, AuthController* auth, QObject *parent = nullptr);

public slots:
    void loadLearnings();
    void addLearning(const StudentLearning& learning);
    void updateLearning(const StudentLearning& learning);
    void deleteLearning(int id);
    void selectLearning(int row);

private:
    MainWindow* m_view;
    AuthController* m_auth;
    StudentLearningRepository m_repo;
    QList<StudentLearning> m_learnings;
};

#endif // LEARNINGCONTROLLER_H