#ifndef USERREPOSITORY_H
#define USERREPOSITORY_H

#include <QList>
#include "models/User.h"

class UserRepository
{
public:
    UserRepository();

    User findById(int id);
    User findByEmail(const QString& email);
    int insert(const User& user);
    bool update(const User& user);
    bool remove(int id);
    bool checkPassword(const QString& email, const QString& password);
};

#endif // USERREPOSITORY_H
