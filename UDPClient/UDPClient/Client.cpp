
#include <iostream>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include<chrono>
#include <ctime>
#include <vector>
// Include the Winsock library (lib) file
#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS
#define DEFAULT_BUFLEN 1024

void main(int argc, char* argv[]) // We can pass in a command line option!! 
{
	////////////////////////////////////////////////////////////
	// INITIALIZE WINSOCK
	////////////////////////////////////////////////////////////

	// Structure to store the WinSock version. This is filled in
	// on the call to WSAStartup()
	WSADATA data;

	// To start WinSock, the required version must be passed to
	// WSAStartup(). This server is going to use WinSock version
	// 2 so I create a word that will store 2 and 2 in hex i.e.
	// 0x0202
	WORD version = MAKEWORD(2, 2);
	char recvbuf[DEFAULT_BUFLEN];
	// Start WinSock
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		// Not ok! Get out quickly
		std::cout << "Can't start Winsock! " << wsOk;
		return;
	}

	////////////////////////////////////////////////////////////
	// CONNECT TO THE SERVER
	////////////////////////////////////////////////////////////

	// Socket creation, note that the socket type is datagram
	SOCKET out = socket(AF_INET, SOCK_DGRAM, 0);

	// Create a hint structure for the server
	sockaddr_in server;
	int severLength = sizeof(server);
	server.sin_family = AF_INET; // AF_INET = IPv4 addresses
	server.sin_port = htons(54000); // Little to big endian conversion
	inet_pton(AF_INET, "127.0.0.1", &server.sin_addr); // Convert from string to byte array


	std::vector<std::string> Messtotal;
	char buf[255];
	
	//user input
	std::string s;

	// temporary variable storing user message  
	std::string Temp;

	// will ask user send a message to sever
	std::cout << "please send message " << std::endl;
	// user message variable 
	std::string mes;
	
	
	// while true get user message 
	do {
		// gets user info 
		std::getline(std::cin, s);

		//if user types sends all user messages sent to sever  
			if (s == "send")

			{
				for (auto mes : Messtotal) {
					// send message to sever 
					int sendMess = sendto(out, mes.c_str(), mes.size() + 1, 0, (sockaddr*)&server, sizeof(server));
					
					// if there is an error in sockect then diconnnect  from sever
					if (sendMess == SOCKET_ERROR) {
						std::cout << "That didn't work! " << WSAGetLastError() << std::endl;

					
						closesocket(sendMess);
						WSACleanup();
						system("pause");
					}
					
				}

				char ServerIp[256];
				ZeroMemory(ServerIp, 256); // Clear the receive buffer
				time_t now = time(0);
				char* dt = ctime(&now);

				// print out date and time of world 
				std::cout << "date and time is:" << dt << std::endl;

				//clear messages 
				Messtotal.clear();
				continue;
			
			}
			//if user input size greater then zero the  mess will have value temporary variable  
			else if (s.size() > 0)
			{
				mes = Temp;
			}

				//if user types exit  send out user last message sent then disconnects from sever  
			if (s == "exit")
			{
				for (auto mes : Messtotal) {
					// send message to sever 
					int sendMess = sendto(out, mes.c_str(), mes.size() + 1, 0, (sockaddr*)&server, sizeof(server));

					// if there is an error in sockect then diconnnect  from sever
					if (sendMess == SOCKET_ERROR)
					{
						std::cout << "That didn't work! " << WSAGetLastError() << std::endl;

					
						closesocket(sendMess);
						WSACleanup();
						break;
					}
				}

				break;

			}
			//store infomation of user input 
			Temp = Temp + "\n" + s + "\n";
			//gets total amount of messages   
			Messtotal.push_back(s + "\n");

	} while (true);

}
