#include <iostream>
#include <WS2tcpip.h>
#include <stdlib.h>
#include <stdio.h>
#include <string>
#include <iostream>
#include <fstream>
#include<chrono>
#include <ctime>
// Include the Winsock library (lib) file
#pragma comment (lib, "ws2_32.lib")
#pragma warning(disable : 4996) //_CRT_SECURE_NO_WARNINGS

bool endsWith(const std::string &mainStr, const std::string &toMatch);
// Main entry point into the server
void main()
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

	// Start WinSock
	int wsOk = WSAStartup(version, &data);
	if (wsOk != 0)
	{
		// Not ok! Get out quickly
		std::cout << "Can't start Winsock! " << wsOk;
		return;
	}

	////////////////////////////////////////////////////////////
	// SOCKET CREATION AND BINDING
	////////////////////////////////////////////////////////////

	// Create a socket, notice that it is a user datagram socket (UDP)
	SOCKET in = socket(AF_INET, SOCK_DGRAM, 0);

	// Create a server hint structure for the server
	sockaddr_in serverHint;
	serverHint.sin_addr.S_un.S_addr = ADDR_ANY; // Us any IP address available on the machine
	serverHint.sin_family = AF_INET; // Address format is IPv4
	serverHint.sin_port = htons(54000); // Convert from little to big endian

										// Try and bind the socket to the IP and port
	if (bind(in, (sockaddr*)&serverHint, sizeof(serverHint)) == SOCKET_ERROR)
	{
		std::cout << "Can't bind socket! " << WSAGetLastError() << std::endl;
		return;
	}

	////////////////////////////////////////////////////////////
	// MAIN LOOP SETUP AND ENTRY
	////////////////////////////////////////////////////////////

	sockaddr_in client; // Use to hold the client information (port / ip address)
	int clientLength = sizeof(client); // The size of the client information

	char buf[1024];
	std::string temp;

	// will conntine to loop until user on client side types exit 
	do {
		ZeroMemory(&client, clientLength); // Clear the client structure
		ZeroMemory(buf, 1024); // Clear the receive buffer

		int bytesIn;
		bytesIn = recvfrom(in, buf, 1024, 0, (sockaddr*)&client, &clientLength);
		

		if (bytesIn == SOCKET_ERROR)
		{
			std::cout << " bytesIn Error receiving from client " << WSAGetLastError() << std::endl;
				continue;
			//break;
		}

		char clientIp[256]; // Create enough space to convert the address byte array
		ZeroMemory(clientIp, 256); // to string of characters

		

		inet_ntop(AF_INET, &client.sin_addr, clientIp, 256);

		time_t now = time(0);
		char* dt = ctime(&now);
		temp = buf;

	

		// Display the message rescived from client with time stamp 
		std::cout << "Message recv from " << clientIp << " : \t\n\n" << buf << "date and time is:\t" << dt << std::endl;

		// if user input ends with exit it will tell sever that client disconnected from sever 
		if (endsWith(temp, "exit\n"))
		{
			std::cout << "Message recv from " << clientIp << " : \t\n\n" << buf << "date and time is:\t" << dt << std::endl;
			std::cout << "Client  has exited" << std::endl;
			system("pause");
			break;
		}

	

	} while (buf != "exit");
	
	//// Close socket
	closesocket(in);
	WSACleanup();
	 ////Shutdown winsock

}
//boolean checks if string matchs user input if so return true else false   
bool endsWith(const std::string &mainStr, const std::string &toMatch)
{
	if (mainStr.size() >= toMatch.size() &&
		mainStr.compare(mainStr.size() - toMatch.size(), toMatch.size(), toMatch) == 0)
		return true;
	else
		return false;
}






