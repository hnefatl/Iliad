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

	bool Send(std::string Message);
	std::string Receive();

    void Shutdown();

private:
    int ServerSocket;
	std::string ServerIP;
	static const int MaxReceiveLength=100;
};

#endif
