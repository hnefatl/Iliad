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

	// Reset data
	memset(&Hints, 0, sizeof(Hints));
	Hints.ai_family=AF_UNSPEC;
	Hints.ai_socktype=SOCK_STREAM;

	int Temp;
	if(Temp=getaddrinfo(argv[1], PORT, &Hints, &ServerInfo)!=0)
	{
		// Failed to get information
		PrintError(Temp);
		return 1;
	}

	// Connect to target
	addrinfo *P;
	for(P=ServerInfo; P!=NULL; P=P->ai_next)
	{
		if((ServerSocket=socket(P->ai_family, P->ai_socktype, P->ai_protocol))==-1)
		{
			PrintError("Could not connect to a target.");
			continue;
		}

		if(connect(ServerSocket, P->ai_addr, P->ai_addrlen)==-1)
		{
			closesocket(ServerSocket);
			PrintError("Client: Connect");
			continue;
		}

		break;
	}

	if(P==NULL)
	{
		// All connections failed
		PrintError("Client connection failed.");
		return 2;
	}

	char s[INET6_ADDRSTRLEN];
	inet_ntop(P->ai_family, P->ai_addr, s, sizeof(s));
	std::cout<<"Connecting to server: "<<s<<std::endl;

	freeaddrinfo(ServerInfo);

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
