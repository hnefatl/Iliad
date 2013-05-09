#include "Client.h"

#include <iostream>
#include <conio.h>
#include <string>
#include <sstream>
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

	return true;
}

bool Client::Send(std::string Message)
{
	std::vector<char> Msg=std::vector<char>(Message.begin(), Message.end());
	int SentTotal=0, Sent=0;

	// Add message size
	std::string Size;
	std::stringstream ss=std::stringstream();
	ss<<Msg.size();
	Size=ss.str();
	// Add size indicator length
	Msg.insert(Msg.begin(), Size.size());
	Msg.insert(Msg.begin()+1, Size.begin(), Size.end());

	while(Msg.size()>0)
	{
		if((Sent = send(ServerSocket, &Msg[0], Msg.size(), 0))==-1)
		{
			return false;
		}
		Msg.erase(Msg.begin(), Msg.begin()+Sent);
	}

	return true;
}
std::string Client::Receive()
{
	int Bytes=0;
	std::vector<char> Buffer;
	std::string Result;

	Buffer.resize(1);

	// Get size of of length of size
	Bytes=recv(ServerSocket, &Buffer[0], 1, 0);
	if(Bytes<=0)
	{
		return "";
	}
	int BytesToGet=Buffer[0];

	Buffer.clear();
	Buffer.resize(BytesToGet);
	// Get length
	Bytes=recv(ServerSocket, &Buffer[0], BytesToGet, 0);
	if(Bytes<=0)
	{
		return "";
	}

	std::stringstream ss=std::stringstream();
	for(int x=0; x<BytesToGet; x++)
	{
		ss<<Buffer[x];
	}

	int PackageBytes=atoi(ss.str().c_str());
	Buffer.clear();
	Buffer.resize(PackageBytes);
	do
	{
		if((Bytes=recv(ServerSocket, &Buffer[0], PackageBytes, 0))<=0)
		{
			return "";
		}
		Result.append(Buffer.begin(), Buffer.end());
	} while(Bytes!=PackageBytes);

	return Result;
}

void Client::Shutdown()
{
	closesocket(ServerSocket);

	WSACleanup();
}