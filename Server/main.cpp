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

std::string RunCommand(std::string Input);

int main(int argc, char *argv[])
{
	RunCommand("echo hello");
	_getch();
	return 0;
}

std::string RunCommand(std::string Input)
{
	system(Input.c_str());
	return "";
}