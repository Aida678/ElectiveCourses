#ifndef PROFILEDIALOG_H
#define PROFILEDIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QVBoxLayout>

class AuthController;

class ProfileDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ProfileDialog(AuthController* auth, QWidget *parent = nullptr);

private slots:
    void onSaveClicked();
    void onCancelClicked();

private:
    AuthController* m_auth;

    QLineEdit* m_oldEmailEdit;
    QLineEdit* m_newEmailEdit;
    QLineEdit* m_newPasswordEdit;
    QLineEdit* m_confirmPasswordEdit;
    QLabel* m_errorLabel;
};

#endif // PROFILEDIALOG_H