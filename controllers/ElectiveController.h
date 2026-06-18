#ifndef ELECTIVECONTROLLER_H
#define ELECTIVECONTROLLER_H

#include <QObject>
#include "repositories/ElectiveRepository.h"
#include "models/Elective.h"

class MainWindow;

class ElectiveController : public QObject
{
    Q_OBJECT

public:
    explicit ElectiveController(MainWindow* view, QObject *parent = nullptr);

public slots:
    void loadElectives();
    void addElective(const Elective& elective);
    void updateElective(const Elective& elective);
    void deleteElective(int id);
    void selectElective(int row);

private:
    MainWindow* m_view;
    ElectiveRepository m_repo;
    QList<Elective> m_electives;
};

#endif // ELECTIVECONTROLLER_H
