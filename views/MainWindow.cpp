#include "MainWindow.h"
#include "ui_mainwindow.h"
#include "database/DatabaseManager.h"
#include "controllers/AuthController.h"
#include "controllers/StudentController.h"
#include "controllers/TeacherController.h"
#include "controllers/ElectiveController.h"
#include "controllers/LearningController.h"
#include "views/ProfileDialog.h"
#include <QMessageBox>
#include <QTableWidgetItem>
#include <QComboBox>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent, AuthController* auth, const User& user)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_currentUser(user)
    , m_auth(auth)
{
    ui->setupUi(this);
    QPushButton* profileBtn = new QPushButton("Профиль");
    profileBtn->setIcon(QIcon::fromTheme("user", QIcon(":/icons/user.png")));
    ui->statusbar->addPermanentWidget(profileBtn);
    connect(profileBtn, &QPushButton::clicked, this, &MainWindow::onProfileClicked);

    setWindowTitle(QString("Управление факультативами - %1 (%2)")
                       .arg(m_currentUser.email())
                       .arg(m_auth ? m_auth->roleDisplayName() : "Без роли"));

    if (m_auth) {
        ui->statusbar->showMessage("Роль: " + m_auth->roleDisplayName());
    }

    if (!DatabaseManager::instance().isOpen()) {
        DatabaseManager::instance().connect("127.0.0.1", "ElectiveCoursesDB", "postgres", "Haroro14", 5433);
    }

    m_studentController = new StudentController(this, m_auth);
    m_teacherController = new TeacherController(this, m_auth);
    m_electiveController = new ElectiveController(this, m_auth);
    m_learningController = new LearningController(this, m_auth);

    // --- Студенты ---
    connect(ui->btnStudentAdd, &QPushButton::clicked, this, &MainWindow::onStudentAddClicked);
    connect(ui->btnStudentUpdate, &QPushButton::clicked, this, &MainWindow::onStudentUpdateClicked);
    connect(ui->btnStudentDelete, &QPushButton::clicked, this, &MainWindow::onStudentDeleteClicked);
    connect(ui->btnStudentClear, &QPushButton::clicked, this, &MainWindow::onStudentClearClicked);
    connect(ui->tableStudent, &QTableWidget::cellClicked, this, &MainWindow::onStudentTableClicked);

    // --- Преподаватели ---
    connect(ui->btnTeacherAdd, &QPushButton::clicked, this, &MainWindow::onTeacherAddClicked);
    connect(ui->btnTeacherUpdate, &QPushButton::clicked, this, &MainWindow::onTeacherUpdateClicked);
    connect(ui->btnTeacherDelete, &QPushButton::clicked, this, &MainWindow::onTeacherDeleteClicked);
    connect(ui->btnTeacherClear, &QPushButton::clicked, this, &MainWindow::onTeacherClearClicked);
    connect(ui->tableTeacher, &QTableWidget::cellClicked, this, &MainWindow::onTeacherTableClicked);

    // --- Факультативы ---
    connect(ui->btnElectiveAdd, &QPushButton::clicked, this, &MainWindow::onElectiveAddClicked);
    connect(ui->btnElectiveUpdate, &QPushButton::clicked, this, &MainWindow::onElectiveUpdateClicked);
    connect(ui->btnElectiveDelete, &QPushButton::clicked, this, &MainWindow::onElectiveDeleteClicked);
    connect(ui->btnElectiveClear, &QPushButton::clicked, this, &MainWindow::onElectiveClearClicked);
    connect(ui->tableElective, &QTableWidget::cellClicked, this, &MainWindow::onElectiveTableClicked);

    // --- Обучение ---
    connect(ui->btnLearningAdd, &QPushButton::clicked, this, &MainWindow::onLearningAddClicked);
    connect(ui->btnLearningUpdate, &QPushButton::clicked, this, &MainWindow::onLearningUpdateClicked);
    connect(ui->btnLearningDelete, &QPushButton::clicked, this, &MainWindow::onLearningDeleteClicked);
    connect(ui->btnLearningClear, &QPushButton::clicked, this, &MainWindow::onLearningClearClicked);
    connect(ui->tableLearning, &QTableWidget::cellClicked, this, &MainWindow::onLearningTableClicked);

    setupGradesTab();
    fillElectiveCombo();
    updateUiForRole();
}

MainWindow::~MainWindow()
{
    delete ui;
    delete m_studentController;
    delete m_teacherController;
    delete m_electiveController;
    delete m_learningController;
}

void MainWindow::updateUiForRole()
{
    if (!m_auth) return;

    bool canEditStudent = m_auth->canCreateStudent() || m_auth->canUpdateStudent() || m_auth->canDeleteStudent();
    ui->btnStudentAdd->setVisible(canEditStudent);
    ui->btnStudentUpdate->setVisible(canEditStudent);
    ui->btnStudentDelete->setVisible(canEditStudent);
    ui->btnStudentClear->setVisible(true);

    bool canEditTeacher = m_auth->canCreateTeacher() || m_auth->canUpdateTeacher() || m_auth->canDeleteTeacher();
    ui->btnTeacherAdd->setVisible(canEditTeacher);
    ui->btnTeacherUpdate->setVisible(canEditTeacher);
    ui->btnTeacherDelete->setVisible(canEditTeacher);
    ui->btnTeacherClear->setVisible(true);

    ui->btnElectiveAdd->setVisible(m_auth->canCreateElective());
    ui->btnElectiveUpdate->setVisible(m_auth->canUpdateElective());
    ui->btnElectiveDelete->setVisible(m_auth->canDeleteElective());
    ui->btnElectiveClear->setVisible(true);

    ui->btnLearningAdd->setVisible(m_auth->canCreateExamSheet());
    ui->btnLearningUpdate->setVisible(m_auth->canUpdateExamSheet());
    ui->btnLearningDelete->setVisible(m_auth->canDeleteExamSheet());
    ui->btnLearningClear->setVisible(true);
}

// ========== Студенты ==========
void MainWindow::onStudentAddClicked()
{
    Student s;
    s.setLastName(ui->lineStudentLastName->text().trimmed());
    s.setFirstName(ui->lineStudentFirstName->text().trimmed());
    s.setMiddleName(ui->lineStudentMiddleName->text().trimmed());
    s.setPhone(ui->lineStudentPhone->text().trimmed());
    s.setAddress(ui->lineStudentAddress->text().trimmed());
    m_studentController->addStudent(s);
}

void MainWindow::onStudentUpdateClicked()
{
    Student s;
    s.setId(ui->lineStudentId->text().toInt());
    s.setLastName(ui->lineStudentLastName->text().trimmed());
    s.setFirstName(ui->lineStudentFirstName->text().trimmed());
    s.setMiddleName(ui->lineStudentMiddleName->text().trimmed());
    s.setPhone(ui->lineStudentPhone->text().trimmed());
    s.setAddress(ui->lineStudentAddress->text().trimmed());
    m_studentController->updateStudent(s);
}

void MainWindow::onStudentDeleteClicked()
{
    int id = ui->lineStudentId->text().toInt();
    m_studentController->deleteStudent(id);
}

void MainWindow::onStudentClearClicked()
{
    clearStudentInputFields();
    showStatusMessage("Поля студентов очищены");
}

void MainWindow::onStudentTableClicked(int row, int column)
{
    Q_UNUSED(column);
    m_studentController->selectStudent(row);
}

void MainWindow::updateStudentTable(const QList<Student>& students)
{
    ui->tableStudent->setRowCount(students.size());
    ui->tableStudent->setColumnCount(6);
    QStringList headers = {"ID", "Фамилия", "Имя", "Отчество", "Телефон", "Адрес"};
    ui->tableStudent->setHorizontalHeaderLabels(headers);
    for (int i = 0; i < students.size(); ++i) {
        const Student& s = students[i];
        ui->tableStudent->setItem(i, 0, new QTableWidgetItem(QString::number(s.id())));
        ui->tableStudent->setItem(i, 1, new QTableWidgetItem(s.lastName()));
        ui->tableStudent->setItem(i, 2, new QTableWidgetItem(s.firstName()));
        ui->tableStudent->setItem(i, 3, new QTableWidgetItem(s.middleName()));
        ui->tableStudent->setItem(i, 4, new QTableWidgetItem(s.phone()));
        ui->tableStudent->setItem(i, 5, new QTableWidgetItem(s.address()));
    }
    ui->tableStudent->resizeColumnsToContents();
    ui->tableStudent->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::clearStudentInputFields()
{
    ui->lineStudentId->clear();
    ui->lineStudentLastName->clear();
    ui->lineStudentFirstName->clear();
    ui->lineStudentMiddleName->clear();
    ui->lineStudentPhone->clear();
    ui->lineStudentAddress->clear();
    ui->btnStudentUpdate->setEnabled(false);
    ui->btnStudentDelete->setEnabled(false);
}

void MainWindow::setStudentInputFields(const Student& s)
{
    ui->lineStudentId->setText(QString::number(s.id()));
    ui->lineStudentLastName->setText(s.lastName());
    ui->lineStudentFirstName->setText(s.firstName());
    ui->lineStudentMiddleName->setText(s.middleName());
    ui->lineStudentPhone->setText(s.phone());
    ui->lineStudentAddress->setText(s.address());
    ui->btnStudentUpdate->setEnabled(true);
    ui->btnStudentDelete->setEnabled(true);
}

// ========== Преподаватели ==========
void MainWindow::onTeacherAddClicked()
{
    Teacher t;
    t.setLastName(ui->lineTeacherLastName->text().trimmed());
    t.setFirstName(ui->lineTeacherFirstName->text().trimmed());
    t.setMiddleName(ui->lineTeacherMiddleName->text().trimmed());
    t.setPhone(ui->lineTeacherPhone->text().trimmed());
    t.setUserId(ui->lineTeacherUserId->text().toInt());
    m_teacherController->addTeacher(t);
}

void MainWindow::onTeacherUpdateClicked()
{
    Teacher t;
    t.setId(ui->lineTeacherId->text().toInt());
    t.setLastName(ui->lineTeacherLastName->text().trimmed());
    t.setFirstName(ui->lineTeacherFirstName->text().trimmed());
    t.setMiddleName(ui->lineTeacherMiddleName->text().trimmed());
    t.setPhone(ui->lineTeacherPhone->text().trimmed());
    t.setUserId(ui->lineTeacherUserId->text().toInt());
    m_teacherController->updateTeacher(t);
}

void MainWindow::onTeacherDeleteClicked()
{
    int id = ui->lineTeacherId->text().toInt();
    m_teacherController->deleteTeacher(id);
}

void MainWindow::onTeacherClearClicked()
{
    clearTeacherInputFields();
    showStatusMessage("Поля преподавателей очищены");
}

void MainWindow::onTeacherTableClicked(int row, int column)
{
    Q_UNUSED(column);
    m_teacherController->selectTeacher(row);
}

void MainWindow::updateTeacherTable(const QList<Teacher>& teachers)
{
    ui->tableTeacher->setRowCount(teachers.size());
    ui->tableTeacher->setColumnCount(6);
    QStringList headers = {"ID", "Фамилия", "Имя", "Отчество", "Телефон", "User ID"};
    ui->tableTeacher->setHorizontalHeaderLabels(headers);
    for (int i = 0; i < teachers.size(); ++i) {
        const Teacher& t = teachers[i];
        ui->tableTeacher->setItem(i, 0, new QTableWidgetItem(QString::number(t.id())));
        ui->tableTeacher->setItem(i, 1, new QTableWidgetItem(t.lastName()));
        ui->tableTeacher->setItem(i, 2, new QTableWidgetItem(t.firstName()));
        ui->tableTeacher->setItem(i, 3, new QTableWidgetItem(t.middleName()));
        ui->tableTeacher->setItem(i, 4, new QTableWidgetItem(t.phone()));
        ui->tableTeacher->setItem(i, 5, new QTableWidgetItem(QString::number(t.userId())));
    }
    ui->tableTeacher->resizeColumnsToContents();
    ui->tableTeacher->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::clearTeacherInputFields()
{
    ui->lineTeacherId->clear();
    ui->lineTeacherLastName->clear();
    ui->lineTeacherFirstName->clear();
    ui->lineTeacherMiddleName->clear();
    ui->lineTeacherPhone->clear();
    ui->lineTeacherUserId->clear();
    ui->btnTeacherUpdate->setEnabled(false);
    ui->btnTeacherDelete->setEnabled(false);
}

void MainWindow::setTeacherInputFields(const Teacher& t)
{
    ui->lineTeacherId->setText(QString::number(t.id()));
    ui->lineTeacherLastName->setText(t.lastName());
    ui->lineTeacherFirstName->setText(t.firstName());
    ui->lineTeacherMiddleName->setText(t.middleName());
    ui->lineTeacherPhone->setText(t.phone());
    ui->lineTeacherUserId->setText(QString::number(t.userId()));
    ui->btnTeacherUpdate->setEnabled(true);
    ui->btnTeacherDelete->setEnabled(true);
}

// ========== Факультативы ==========
void MainWindow::onElectiveAddClicked()
{
    Elective e;
    e.setName(ui->lineElectiveName->text().trimmed());
    e.setDepartmentId(ui->lineElectiveDepartmentId->text().toInt());
    m_electiveController->addElective(e);
}

void MainWindow::onElectiveUpdateClicked()
{
    Elective e;
    e.setId(ui->lineElectiveId->text().toInt());
    e.setName(ui->lineElectiveName->text().trimmed());
    e.setDepartmentId(ui->lineElectiveDepartmentId->text().toInt());
    m_electiveController->updateElective(e);
}

void MainWindow::onElectiveDeleteClicked()
{
    int id = ui->lineElectiveId->text().toInt();
    m_electiveController->deleteElective(id);
}

void MainWindow::onElectiveClearClicked()
{
    clearElectiveInputFields();
    showStatusMessage("Поля факультативов очищены");
}

void MainWindow::onElectiveTableClicked(int row, int column)
{
    Q_UNUSED(column);
    m_electiveController->selectElective(row);
}

void MainWindow::updateElectiveTable(const QList<Elective>& electives)
{
    ui->tableElective->setRowCount(electives.size());
    ui->tableElective->setColumnCount(3);
    QStringList headers = {"ID", "Название", "Кафедра ID"};
    ui->tableElective->setHorizontalHeaderLabels(headers);
    for (int i = 0; i < electives.size(); ++i) {
        const Elective& e = electives[i];
        ui->tableElective->setItem(i, 0, new QTableWidgetItem(QString::number(e.id())));
        ui->tableElective->setItem(i, 1, new QTableWidgetItem(e.name()));
        ui->tableElective->setItem(i, 2, new QTableWidgetItem(QString::number(e.departmentId())));
    }
    ui->tableElective->resizeColumnsToContents();
    ui->tableElective->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::clearElectiveInputFields()
{
    ui->lineElectiveId->clear();
    ui->lineElectiveName->clear();
    ui->lineElectiveDepartmentId->clear();
    ui->btnElectiveUpdate->setEnabled(false);
    ui->btnElectiveDelete->setEnabled(false);
}

void MainWindow::setElectiveInputFields(const Elective& e)
{
    ui->lineElectiveId->setText(QString::number(e.id()));
    ui->lineElectiveName->setText(e.name());
    ui->lineElectiveDepartmentId->setText(QString::number(e.departmentId()));
    ui->btnElectiveUpdate->setEnabled(true);
    ui->btnElectiveDelete->setEnabled(true);
}

// ========== Обучение ==========
void MainWindow::onLearningAddClicked()
{
    StudentLearning l;
    l.setGrade(ui->lineLearningGrade->text().toInt());
    l.setExamDate(ui->dateLearningExam->date());
    l.setTeacherId(ui->lineLearningTeacherId->text().toInt());
    l.setSemesterCourseId(ui->lineLearningSemesterCourseId->text().toInt());
    l.setStudentId(ui->lineLearningStudentId->text().toInt());
    m_learningController->addLearning(l);
}

void MainWindow::onLearningUpdateClicked()
{
    StudentLearning l;
    l.setId(ui->lineLearningId->text().toInt());
    l.setGrade(ui->lineLearningGrade->text().toInt());
    l.setExamDate(ui->dateLearningExam->date());
    l.setTeacherId(ui->lineLearningTeacherId->text().toInt());
    l.setSemesterCourseId(ui->lineLearningSemesterCourseId->text().toInt());
    l.setStudentId(ui->lineLearningStudentId->text().toInt());
    m_learningController->updateLearning(l);
}

void MainWindow::onLearningDeleteClicked()
{
    int id = ui->lineLearningId->text().toInt();
    m_learningController->deleteLearning(id);
}

void MainWindow::onLearningClearClicked()
{
    clearLearningInputFields();
    showStatusMessage("Поля обучения очищены");
}

void MainWindow::onLearningTableClicked(int row, int column)
{
    Q_UNUSED(column);
    m_learningController->selectLearning(row);
}

void MainWindow::updateLearningTable(const QList<StudentLearning>& learnings)
{
    ui->tableLearning->setRowCount(learnings.size());
    ui->tableLearning->setColumnCount(6);
    QStringList headers = {"ID", "Оценка", "Дата", "Teacher ID", "SemCourse ID", "Student ID"};
    ui->tableLearning->setHorizontalHeaderLabels(headers);
    for (int i = 0; i < learnings.size(); ++i) {
        const StudentLearning& l = learnings[i];
        ui->tableLearning->setItem(i, 0, new QTableWidgetItem(QString::number(l.id())));
        ui->tableLearning->setItem(i, 1, new QTableWidgetItem(QString::number(l.grade())));
        ui->tableLearning->setItem(i, 2, new QTableWidgetItem(l.examDate().toString("dd.MM.yyyy")));
        ui->tableLearning->setItem(i, 3, new QTableWidgetItem(QString::number(l.teacherId())));
        ui->tableLearning->setItem(i, 4, new QTableWidgetItem(QString::number(l.semesterCourseId())));
        ui->tableLearning->setItem(i, 5, new QTableWidgetItem(QString::number(l.studentId())));
    }
    ui->tableLearning->resizeColumnsToContents();
    ui->tableLearning->horizontalHeader()->setStretchLastSection(true);
}

void MainWindow::clearLearningInputFields()
{
    ui->lineLearningId->clear();
    ui->lineLearningGrade->clear();
    ui->dateLearningExam->setDate(QDate::currentDate());
    ui->lineLearningTeacherId->clear();
    ui->lineLearningSemesterCourseId->clear();
    ui->lineLearningStudentId->clear();
    ui->btnLearningUpdate->setEnabled(false);
    ui->btnLearningDelete->setEnabled(false);
}

void MainWindow::setLearningInputFields(const StudentLearning& l)
{
    ui->lineLearningId->setText(QString::number(l.id()));
    ui->lineLearningGrade->setText(QString::number(l.grade()));
    ui->dateLearningExam->setDate(l.examDate());
    ui->lineLearningTeacherId->setText(QString::number(l.teacherId()));
    ui->lineLearningSemesterCourseId->setText(QString::number(l.semesterCourseId()));
    ui->lineLearningStudentId->setText(QString::number(l.studentId()));
    ui->btnLearningUpdate->setEnabled(true);
    ui->btnLearningDelete->setEnabled(true);
}

void MainWindow::showStatusMessage(const QString& msg, bool isError)
{
    if (isError)
        ui->statusbar->setStyleSheet("color: red;");
    else
        ui->statusbar->setStyleSheet("");
    ui->statusbar->showMessage(msg, 5000);
}

void MainWindow::setupGradesTab()
{
    m_tabGrades = new QWidget();
    ui->tabWidget->addTab(m_tabGrades, "Ведомость");

    QVBoxLayout* layout = new QVBoxLayout(m_tabGrades);

    QHBoxLayout* topLayout = new QHBoxLayout();
    QLabel* label = new QLabel("Факультатив:");
    m_comboElective = new QComboBox();
    m_comboElective->addItem("— Выберите факультатив —", 0);
    QPushButton* btnLoad = new QPushButton("Загрузить");
    topLayout->addWidget(label);
    topLayout->addWidget(m_comboElective);
    topLayout->addWidget(btnLoad);
    topLayout->addStretch();

    m_tableGrades = new QTableWidget();
    m_tableGrades->setColumnCount(3);
    m_tableGrades->setHorizontalHeaderLabels({"ID", "Студент", "Итоговая оценка"});
    m_tableGrades->horizontalHeader()->setStretchLastSection(true);

    layout->addLayout(topLayout);
    layout->addWidget(m_tableGrades);

    connect(btnLoad, &QPushButton::clicked, this, &MainWindow::onLoadGradesClicked);
    connect(m_comboElective, QOverload<int>::of(&QComboBox::currentIndexChanged),
            this, &MainWindow::onElectiveChanged);
}

void MainWindow::fillElectiveCombo()
{
    m_comboElective->clear();
    m_comboElective->addItem("— Выберите факультатив —", 0);
    QList<Elective> electives = m_electiveController->getAllElectives();
    for (const Elective& e : electives) {
        m_comboElective->addItem(e.name(), e.id());
    }
}

void MainWindow::onLoadGradesClicked()
{
    int electiveId = m_comboElective->currentData().toInt();
    loadGradesForElective(electiveId);
}

void MainWindow::onElectiveChanged(int index)
{
    Q_UNUSED(index);
    // Можно загружать автоматически при выборе, но мы оставим по кнопке
}

void MainWindow::loadGradesForElective(int electiveId)
{
    if (electiveId == 0) {
        m_tableGrades->setRowCount(0);
        return;
    }
    QList<StudentGrade> grades = m_electiveController->getStudentGradesForElective(electiveId);
    updateGradesTable(grades);
}

void MainWindow::updateGradesTable(const QList<StudentGrade>& grades)
{
    m_tableGrades->setRowCount(grades.size());
    for (int i = 0; i < grades.size(); ++i) {
        const StudentGrade& g = grades[i];
        m_tableGrades->setItem(i, 0, new QTableWidgetItem(QString::number(g.studentId())));
        m_tableGrades->setItem(i, 1, new QTableWidgetItem(g.fullName()));
        m_tableGrades->setItem(i, 2, new QTableWidgetItem(g.finalGrade() > 0 ? QString::number(g.finalGrade()) : "—"));
    }
    m_tableGrades->resizeColumnsToContents();
}

void MainWindow::onProfileClicked()
{
    ProfileDialog dialog(m_auth, this);
    dialog.exec();
}