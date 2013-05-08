#ifndef _CLIENT_H
#define _CLIENT_H

#include <string>

#include <WS2tcpip.h>
#include <winsock.h>
#include <Windows.h>

class Client
{
public:
    Client();

    bool Connect(std::string Target, std::string Port);

    void Shutdown();

	bool Send(std::string Message);
	std::string Recieve();

private:
	static const int MaxRecieveLength=100;
    int ServerSocket;
	std::string ServerIP;
};

#endif
