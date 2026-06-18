#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "models/User.h"
#include "models/Student.h"
#include "models/Teacher.h"
#include "models/Elective.h"
#include "models/SemesterCourse.h"
#include "models/Lesson.h"
#include "models/StudentLearning.h"

namespace Ui { class MainWindow; }

class StudentController;
class TeacherController;
class ElectiveController;
class LearningController;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr, const User& user = User());
    ~MainWindow();

    // Методы для студентов
    void updateStudentTable(const QList<Student>& students);
    void clearStudentInputFields();
    void setStudentInputFields(const Student& student);

    // Методы для преподавателей
    void updateTeacherTable(const QList<Teacher>& teachers);
    void clearTeacherInputFields();
    void setTeacherInputFields(const Teacher& teacher);

    // Методы для факультативов
    void updateElectiveTable(const QList<Elective>& electives);
    void clearElectiveInputFields();
    void setElectiveInputFields(const Elective& elective);

    // Методы для обучения
    void updateLearningTable(const QList<StudentLearning>& learnings);
    void clearLearningInputFields();
    void setLearningInputFields(const StudentLearning& learning);

    void showStatusMessage(const QString& msg, bool isError = false);

private slots:
    // Студенты
    void onStudentAddClicked();
    void onStudentUpdateClicked();
    void onStudentDeleteClicked();
    void onStudentClearClicked();
    void onStudentTableClicked(int row, int column);

    // Преподаватели
    void onTeacherAddClicked();
    void onTeacherUpdateClicked();
    void onTeacherDeleteClicked();
    void onTeacherClearClicked();
    void onTeacherTableClicked(int row, int column);

    // Факультативы
    void onElectiveAddClicked();
    void onElectiveUpdateClicked();
    void onElectiveDeleteClicked();
    void onElectiveClearClicked();
    void onElectiveTableClicked(int row, int column);

    // Обучение
    void onLearningAddClicked();
    void onLearningUpdateClicked();
    void onLearningDeleteClicked();
    void onLearningClearClicked();
    void onLearningTableClicked(int row, int column);

private:
    Ui::MainWindow *ui;
    User m_currentUser;
    StudentController* m_studentController;
    TeacherController* m_teacherController;
    ElectiveController* m_electiveController;
    LearningController* m_learningController;
};

#endif // MAINWINDOW_H
