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

#include "Client.h"

void Exit();

Client client;

int main(int argc, char *argv[])
{
	// Register exit function
	atexit(Exit);

	std::string IP, Port;
	std::cout<<"Enter Target IP: ";
	std::getline(std::cin, IP);
	std::cout<<"Enter Target Port: ";
	std::getline(std::cin, Port);
	system("cls");
	client=Client();

	if(!client.Connect(IP, Port))
	{
		// Connection failed
		std::cout<<"Could not connect to "<<IP<<":"<<Port<<". Press any key to exit..."<<std::endl;
		_getch();
		return 1;
	}

	// Command loop
	std::string Input;
	while(true)
	{
		std::cout<<"> ";
		std::getline(std::cin, Input);

		if(Input=="exit")
		{
			break;
		}
		else
		{
			client.Send(Input);
			std::string Recieved=client.Recieve();
			if(Recieved=="")
			{

			}
			else
			{
				std::cout<<Recieved;
			}
		}
	}

	client.Shutdown();

	return 0;
}

void Exit()
{
	client.Shutdown();
}