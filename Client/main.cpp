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

std::string SendCommand(std::string Command);
void Shutdown();
void PrintError(int ErrCode);
void PrintError(std::string Error);

#define PORT "597"
const unsigned int MAX_DATA_SIZE=4096;
int ServerSocket;
addrinfo Hints, *ServerInfo;

int main(int argc, char *argv[])
{
	if(argc!=2)
	{
		std::cout<<"Usage: Client.exe :TargetIP\n";
		return 1;
	}
	
	// Command loop
	std::string Input;
	while (true)
	{
		std::getline(std::cin, Input);

		if(Input=="exit")
		{
			Shutdown();
			break;
		}
		else
		{
			std::cout<<SendCommand(Input);
		}
	}


	return 0;
}

std::string SendCommand(std::string Command)
{
	return "";
}

void Shutdown()
{

}

void PrintError(int ErrCode)
{
	std::cout<<"Error: "<<std::endl<<gai_strerror(ErrCode)<<std::endl;
}
void PrintError(std::string Error)
{
	std::cout<<Error<<std::cout;
}
