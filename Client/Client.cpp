#include "Client.h"

#include <iostream>
#include <conio.h>
#include <string>
#include <vector>
#include <algorithm>

#include <WS2tcpip.h>
#include <winsock.h>
#include <Windows.h>

Client::Client()
{
	
}

bool Client::Connect(std::string Target, std::string Port)
{
	WSAData Data;

	if(WSAStartup(MAKEWORD(1, 1), &Data)!=0)
	{
		return false;
	}

	addrinfo *ServerInfo, Hints;

	memset(&Hints, 0, sizeof(Hints));
	Hints.ai_family=AF_UNSPEC;
	Hints.ai_socktype=SOCK_STREAM;

	int Rcv;
	if((Rcv=getaddrinfo(Target.c_str(), Port.c_str(), &Hints, &ServerInfo))!=0)
	{
		return false;
	}

	addrinfo *p;
	for(p=ServerInfo; p!=NULL; p=ServerInfo->ai_next)
	{
		if((ServerSocket=socket(p->ai_family, p->ai_socktype, p->ai_protocol))==-1)
		{
			continue;
		}

		if(connect(ServerSocket, p->ai_addr, p->ai_addrlen)==-1)
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

	char s[INET6_ADDRSTRLEN];
	inet_ntop(p->ai_family, p->ai_addr, s, sizeof(s));
	ServerIP=std::string(s);
	freeaddrinfo(ServerInfo);
	freeaddrinfo(p);

	return true;
}

bool Client::Send(std::string Message)
{
	std::vector<char> Msg=std::vector<char>(Message.begin(), Message.end());
	int SentTotal=0, Sent=0, Total=Msg.size();

	while(SentTotal!=Total)
	{
		if(Sent += (send(ServerSocket, &Msg[0], Msg.size(), 0))==-1)
		{
			return false;
		}
		Msg.erase(Msg.begin(), Msg.begin()+Sent);
		SentTotal+=Sent;
	}
	
	return true;
}
std::string Client::Receive()
{
	int Bytes=0;
	char Buffer[MaxReceiveLength];
	std::string Result;

	do
	{
		if((Bytes=recv(ServerSocket, Buffer, MaxReceiveLength, 0))==-1)
		{
			return "";
		}
		Result+=Buffer;
	} while(Bytes>0);

	return Result;
}

void Client::Shutdown()
{
	closesocket(ServerSocket);

	WSACleanup();
}