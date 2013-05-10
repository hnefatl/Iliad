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

	while(true)
	{
		std::string IP, Port;
		std::cout<<"Enter Target IP: ";
		std::getline(std::cin, IP);
		std::cout<<"Enter Target Port: ";
		std::getline(std::cin, Port);
		system("cls");
		std::cout<<"Attempting to connect...";
		client=Client();

		if(!client.Connect(IP, Port))
		{
			// Connection failed
			system("cls");
			std::cout<<"Could not connect to "<<IP<<"|"<<Port<<". Press any key to exit..."<<std::endl;
			_getch();
			return 1;
		}

		// Command loop
		system("cls");
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
				std::string Received=client.Receive();
				if(Received=="/serverdisconnected")
				{
					std::cout<<"The server was closed."<<std::endl;
					client.Shutdown();
					break;
				}
				else if(Received=="/exit")
				{
					client.Shutdown();
					break;
				}
				else
				{
					std::cout<<Received;
				}
			}
		}
	}

	return 0;
}

void Exit()
{
	client.Shutdown();
}