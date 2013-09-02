#include "StdAfx.h"

#include "Pack_LoginCL.h"
#include "Pack_LoginSV.h"
#include "User.h"


Pack_LoginCL::Pack_LoginCL()
{
}

Pack_LoginCL::~Pack_LoginCL()
{
}

void Pack_LoginCL::In(PkIStream & stream)
{
	stream.Read(mAccount, MAX_ACCOUNT_SIZE);
	stream.Read(mPassword, MAX_PASSWORD_SIZE);
}

void Pack_LoginCL::Execute(void * param)
{
    d_assert(param);

    User * user = (User *)param;

    Pack_LoginSV pk;

    int result = eLoginFail;

    if (strcmp(mAccount, "test2001") == 0)
    {
        if (strcmp(mPassword, "123456") == 0)
        {
            result = eLoginOk;
        }
        else
        {
            result = eLoginPasswardError;
        }
    }
    else
    {
        result = eLoginAccountError;
    }

    pk.SetResult(result);

    user->SendPacket(&pk);
    user->SetState(User::LOGIN);
}
