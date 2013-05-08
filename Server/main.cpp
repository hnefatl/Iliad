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
	// Hide window
	ShowWindow(GetConsoleWindow(), SW_HIDE);



	return 0;
}

std::string RunCommand(std::string Input)
{
	FILE *Pipe=_popen(Input.c_str(), "r");
	if(!Pipe)
	{
		return "Error";
	}
	char Buffer[256];
	std::string Result="";
	while(!feof(Pipe))
	{
		if(fgets(Buffer, 256, Pipe)!=NULL)
		{
			Result+=Buffer;
		}
	}
	_pclose(Pipe);
	return Result;
}