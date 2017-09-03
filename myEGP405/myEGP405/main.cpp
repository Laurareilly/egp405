#include <iostream>
#include <stdio.h>
#include <string>
#include "../../../DevSDKs/DevSDKs/include/RakNet/RakPeerInterface.h" //this needs to be fixed
#include "../../../DevSDKs/DevSDKs/include/RakNet/MessageIdentifiers.h" //so does this

using namespace std;
using namespace RakNet;

int main()
{
	unsigned int maxClients = 10;
	unsigned short serverPort = 60000;
	RakPeerInterface *peer = RakPeerInterface::GetInstance();
	bool isServer;
	Packet *packet;

	string choice;
	cout << "(C) or (S)erver?\n";
	getline(cin, choice);

	cout << "Enter the server port number\n";
	string serverPortString;
	getline(cin, serverPortString);
	serverPort = stoi(serverPortString);

	if (choice == "c" || choice == "C") //if we're starting a client
	{
		SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		isServer = false;
	}
	else //if we're starting a server
	{
		cout << "Enter the max amount of clients\n";
		string maxClientsString; 
		getline(cin, maxClientsString);
		maxClients = stoi(maxClientsString);
		SocketDescriptor sd(serverPort, 0);
		peer->Startup(maxClients, &sd, 1);
		isServer = true;
	}

	if (isServer)
	{
		cout << "Starting the server\n";
		peer->SetMaximumIncomingConnections(maxClients); //making sure no more than the max can connect to the server
	}
	else
	{
		string answer;
		cout << "Enter the server IP or hit enter for 127.0.0.1\n";
		getline(cin, answer);
		if (answer == "")
		{
			//answer = "127.0.0.1";
			answer = "10.0.0.1"; //this one is my house
		}
		cout << "Starting the client\n";
		peer->Connect(answer.c_str(), serverPort, 0, 0); //these 2 zeros are for password data and length, guessing they're 0 cus we don't have any
	}

	while (1)
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				cout << "Another client has disconnected.\n";
				break;
			case ID_REMOTE_CONNECTION_LOST:
				cout << "Another client has lost the connection.\n";
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				cout << "Another client has connected.\n";
				break;
			case ID_CONNECTION_REQUEST_ACCEPTED:
				cout << "Our connection request has been accepted.\n";
				break;
			case ID_NEW_INCOMING_CONNECTION:
				cout << "A connection is incoming.\n";
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				cout << "The server is full.\n";
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				if (isServer)
				{
					cout << "A client has disconnected.\n";
				}
				else
				{
					cout << "We have been disconnected.\n";
				}
				break;
			case ID_CONNECTION_LOST:
				if (isServer)
				{
					cout << "A client lost the connection.\n";
				}
				else
				{
					cout << "Connection lost.\n";
				}
				break;
			default:
				cout << "Message with identifier " << packet->data[0] << " has arrived.\n";
				break;
			}
		}
	}

	RakPeerInterface::DestroyInstance(peer);

	cin.get();
	cin.ignore();
	return 0;
}