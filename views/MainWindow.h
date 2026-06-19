#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "models/User.h"
#include "models/Student.h"
#include "models/Teacher.h"
#include "models/Elective.h"
#include "models/StudentLearning.h"
#include "models/StudentGrade.h"
#include <QComboBox>
#include <QTableWidget>
#include <QAction>

namespace Ui { class MainWindow; }

class AuthController;
class StudentController;
class TeacherController;
class ElectiveController;
class LearningController;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, AuthController* auth = nullptr, const User& user = User());
    ~MainWindow();

    void updateStudentTable(const QList<Student>& students);
    void clearStudentInputFields();
    void setStudentInputFields(const Student& student);

    void updateTeacherTable(const QList<Teacher>& teachers);
    void clearTeacherInputFields();
    void setTeacherInputFields(const Teacher& teacher);

    void updateElectiveTable(const QList<Elective>& electives);
    void clearElectiveInputFields();
    void setElectiveInputFields(const Elective& elective);

    void updateLearningTable(const QList<StudentLearning>& learnings);
    void clearLearningInputFields();
    void setLearningInputFields(const StudentLearning& learning);

    void showStatusMessage(const QString& msg, bool isError = false);

    void updateUiForRole();   // обновление интерфейса по роли

private slots:
    void onStudentAddClicked();
    void onStudentUpdateClicked();
    void onStudentDeleteClicked();
    void onStudentClearClicked();
    void onStudentTableClicked(int row, int column);

    void onTeacherAddClicked();
    void onTeacherUpdateClicked();
    void onTeacherDeleteClicked();
    void onTeacherClearClicked();
    void onTeacherTableClicked(int row, int column);

    void onElectiveAddClicked();
    void onElectiveUpdateClicked();
    void onElectiveDeleteClicked();
    void onElectiveClearClicked();
    void onElectiveTableClicked(int row, int column);

    void onLearningAddClicked();
    void onLearningUpdateClicked();
    void onLearningDeleteClicked();
    void onLearningClearClicked();
    void onLearningTableClicked(int row, int column);

    void onLoadGradesClicked();
    void onElectiveChanged(int index);
    void onProfileClicked();

private:
    Ui::MainWindow *ui;
    User m_currentUser;
    AuthController* m_auth;
    StudentController* m_studentController;
    TeacherController* m_teacherController;
    ElectiveController* m_electiveController;
    LearningController* m_learningController;
    QWidget* m_tabGrades;
    QComboBox* m_comboElective;
    QTableWidget* m_tableGrades;
    QAction* m_profileAction;
    void setupGradesTab();
    void loadGradesForElective(int electiveId);
    void updateGradesTable(const QList<StudentGrade>& grades);
    void fillElectiveCombo();
};

#endif // MAINWINDOW_H