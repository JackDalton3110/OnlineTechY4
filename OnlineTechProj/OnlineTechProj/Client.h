#pragma once
#include <iostream>
#include <string>
#include <WS2tcpip.h>
#include <vector>
#pragma comment(lib, "ws2_32.lib")


using namespace std;


class Client {
public:
	Client();
	bool run();
	void recieve();
	void sendMessage(std::string message);
	void close();
	std::vector <float> posVector;
	int clientNum = 0;
	bool join;
	bool gameOver = false;

private:

	SOCKET sock;
	sockaddr_in hint;
	WSAData data;
	WORD ver;
	string ipAddress;
	int port;
	char buf [4096];
	int bytesReceived;
};