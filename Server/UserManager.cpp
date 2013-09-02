#include "StdAfx.h"

#include "UserManager.h"


UserManager::UserManager()
{
    mUserSize = 0;
}

UserManager::~UserManager()
{
}

User * UserManager::CreateUser()
{
    for (int i = 0; i < MAX_USER; ++i)
    {
        if (!mUsers[i].IsValid())
        {
            mUsers[i].mUserId = i;
            mUsers[i].Init();

            ++mUserSize;

            return &mUsers[i];
        }
    }

    return NULL;
}

void UserManager::DestroyUser(User * user)
{
    DestroyUser(user->GetUserId());
}

void UserManager::DestroyUser(int user)
{
    if (mUsers[user].IsValid())
    {
        mUsers[user].Shutdown();
        --mUserSize;
    }
}

User * UserManager::GetUser(int user)
{
    if (user == User::INVALIDE_USER_ID || user < 0 || user > MAX_USER)
        return NULL;

    User * u = &mUsers[user];

    return u->IsValid() ? u : NULL;
}

void UserManager::DestroyAllUser()
{
    for (int i = 0; i < MAX_USER; ++i)
    {
        if (mUsers[i].IsValid())
        {
            mUsers[i].Shutdown();
        }
    }

    mUserSize = 0;
}

int UserManager::GetUserSize() const
{
    return mUserSize;
}
