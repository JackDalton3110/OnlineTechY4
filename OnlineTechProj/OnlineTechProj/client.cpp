#include "Client.h"
	Client::Client()
	{
		 ipAddress = "149.153.106.161";         // IP Address of the server
		 port = 54000;                       // Listening port # on the server

												// Initialize WinSock
		 ver = MAKEWORD(2, 2);
		int wsResult = WSAStartup(ver, &data);
		if (wsResult != 0)
		{
			cerr << "Can't start Winsock, Err #" << wsResult << endl;
			return;
		}

		// Create socket
		sock = socket(AF_INET, SOCK_STREAM, 0);
		if (sock == INVALID_SOCKET)
		{
			cerr << "Can't create socket, Err #" << WSAGetLastError() << endl;
			WSACleanup();
			return;
		}

		// Fill in a hint structure
		hint;
		hint.sin_family = AF_INET;
		hint.sin_port = htons(port);
		inet_pton(AF_INET, ipAddress.c_str(), &hint.sin_addr);

		
	}

	bool Client::run()
	{
		// Connect to server
		int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
		if (connResult == SOCKET_ERROR)
		{
			cerr << "Can't connect to server, Err #" << WSAGetLastError() << endl;
			closesocket(sock);
			WSACleanup();
			return false;
		}
		u_long iMode = 1;
		ioctlsocket(sock, FIONBIO, &iMode);
		return true;
	}

	void Client::recieve()
	{
			// Do-while loop to send and receive data
			// Wait for response
			ZeroMemory(buf, 4096);
			bytesReceived = recv(sock, buf, 4096, 0);
			if (bytesReceived > 0)
			{
				posVector.clear();
				// Echo response to console
				cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;

				std::string pos2 = string(buf, 0, bytesReceived);
				size_t pos = 0;
				std::string token;

				std::string delimiter = "/";
				pos = pos2.find(delimiter);
				token = pos2.substr(0, pos);
				pos2.erase(0, pos + delimiter.length());

				if (token == "Position")
				{
					while ((pos = pos2.find(delimiter)) != std::string::npos)
					{
						token = pos2.substr(0, pos);
						posVector.push_back(atof(token.c_str()));
						pos2.erase(0, pos + delimiter.length());
					}
				}

				if (token == "Welcome to the Awesome Chat Server player:")
				{
					while ((pos = pos2.find(delimiter)) != std::string::npos)
					{
						token = pos2.substr(0, pos);
						clientNum = atof(token.c_str());
						join = true;
						pos2.erase(0, pos + delimiter.length());
					}
				}

				if (token == "EndGame")
				{
					gameOver = true;
				}

			}
		
	}

	void Client::sendMessage(std::string message)
	{
		string userInput;
		userInput = message;
		int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
	}

	void Client::close()
	{
		// Gracefully close down everything
		closesocket(sock);
		WSACleanup();

	}
	