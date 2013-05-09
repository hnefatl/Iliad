#include <iostream>

#ifndef _WIN32
int main(int argc, char *argv[])
{
	return 1;
}
#endif

#include <conio.h>
#include <string>
#include <algorithm>

#include <WS2tcpip.h>
#include <winsock.h>
#include <Windows.h>

#include "Server.h"

std::string RunCommand(std::string Input);

int main(int argc, char *argv[])
{
	// Hide window
	//ShowWindow(GetConsoleWindow(), SW_HIDE);

	Server server=Server();
	if(!server.Bind("34652"))
	{
		return 1;
	}
	server.Start();

	return 0;
}