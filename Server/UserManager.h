#pragma once

#include "User.h"

class UserManager
{
public:
    UserManager();
    ~UserManager();

    User * CreateUser();
    User * GetUser(int user);

    void DestroyUser(User * user);
    void DestroyUser(int user);
    void DestroyAllUser();

    int GetUserSize() const;

protected:
    User mUsers[MAX_USER];
    int mUserSize;
};
