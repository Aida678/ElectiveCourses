QT += core gui sql widgets

SOURCES += \
    main.cpp \
    views/MainWindow.cpp \
    views/LoginWindow.cpp \
    controllers/AuthController.cpp \
    controllers/StudentController.cpp \
    controllers/TeacherController.cpp \
    controllers/ElectiveController.cpp \
    controllers/LearningController.cpp \
    database/DatabaseManager.cpp \
    models/Student.cpp \
    models/User.cpp \
    models/Teacher.cpp \
    models/Elective.cpp \
    models/SemesterCourse.cpp \
    models/Lesson.cpp \
    models/StudentLearning.cpp \
    repositories/StudentRepository.cpp \
    repositories/UserRepository.cpp \
    repositories/TeacherRepository.cpp \
    repositories/ElectiveRepository.cpp \
    repositories/SemesterCourseRepository.cpp \
    repositories/LessonRepository.cpp \
    repositories/StudentLearningRepository.cpp \
    views/RegisterWindow.cpp

HEADERS += \
    views/MainWindow.h \
    views/LoginWindow.h \
    controllers/AuthController.h \
    controllers/StudentController.h \
    controllers/TeacherController.h \
    controllers/ElectiveController.h \
    controllers/LearningController.h \
    database/DatabaseManager.h \
    models/Student.h \
    models/User.h \
    models/Teacher.h \
    models/Elective.h \
    models/SemesterCourse.h \
    models/Lesson.h \
    models/StudentLearning.h \
    repositories/StudentRepository.h \
    repositories/UserRepository.h \
    repositories/TeacherRepository.h \
    repositories/ElectiveRepository.h \
    repositories/SemesterCourseRepository.h \
    repositories/LessonRepository.h \
    repositories/StudentLearningRepository.h \
    views/RegisterWindow.h

FORMS += \
    views/loginwindow.ui \
    views/mainwindow.ui \
    views/registerwindow.ui

DISTFILES += \
    .gitignore
