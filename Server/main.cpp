#include "StdAfx.h"

#include "User.h"
#include "Server.h"

int main()
{
    Server * sv = new Server;

	sv->Run();

	delete sv;

	std::cin.get();

    return 0;
}