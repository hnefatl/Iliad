#include "Client.h"

#include <iostream>
#include <conio.h>
#include <string>
#include <algorithm>

#include <WS2tcpip.h>
#include <winsock.h>
#include <Windows.h>

Client::Client()
{

}

bool Client::Connect(std::string Target, std::string Port)
{
    addrinfo Hints;

    memset(&Hints, 0, sizeof(Hints));
    Hints.ai_family=AF_UNSPEC;
    Hints.ai_socktype=SOCK_STREAM;

    int Rcv;
    if((Rcv=getaddrinfo(Target, Port, &Hints, &ServerInfo))!=0)
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
}
