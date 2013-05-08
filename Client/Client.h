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

private:
    int ServerSocket;
    addrinfo *ServerInfo;
};

#endif
