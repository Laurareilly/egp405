/*
This file was created by Laura Reilly (0972157) using tutorials from http://www.jenkinssoftware.com/raknet/manual/tutorialsample3.html 
I certify that this work is entirely my own.  
The assessor of this project may reproduce this project 
and provide copies to other academic staff, and/or communicate 
a copy of this project to a plagiarism-checking service, 
which may retain a copy of the project on its database.”
*/

#include "PacketStructs.h"
#include <iostream>
#include <stdio.h>
#include <conio.h>

using namespace std;

int main()
{
	unsigned int maxClients = 10;
	unsigned short serverPort = 60000;
	RakPeerInterface *peer = RakPeerInterface::GetInstance();
	bool isServer;
	Packet *packet;
	string username; 

	string choice;
	cout << "(C) or (S)erver?\n";
	getline(cin, choice);

	cout << "Enter the server port number\n";
	string serverPortString;
	getline(cin, serverPortString);
	if (serverPortString == "") serverPortString = "60000";
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
		if (maxClientsString == "") maxClientsString = "10";
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
			answer = "127.0.0.1";
		}
		
		cout << "Starting the client\n";
		peer->Connect(answer.c_str(), serverPort, 0, 0); //these 2 zeros are for password data and length, guessing they're 0 cus we don't have any

		cout << "Enter a username" << endl;
		getline(cin, username);
	}

	while (true)
	{
		if (_kbhit() && isServer == false) //will wait for a key press, downside is you can't receive messages while you're typing
										  // not sure what that solve would really be
		{
			//clients can type in a message and it'll store what they say in the struct's data
			UserMessage message;
			string tempMessageString;
			cout << username << ": ";
			getline(cin, tempMessageString);
			message.message = tempMessageString.c_str();
			message.userName = username.c_str();
			message.timeStamp = GetTime();

			BitStream bsOut;
			bsOut.Write((MessageID)ID_MESSAGE_SENT);
			//bsOut.Write((char*)&message, sizeof(message));
			bsOut.Write(message.userName);
			bsOut.Write(message.message);
			bsOut.Write(message.timeStamp);
			peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
		}

		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive()) //gross for loop basically initiates a packet, makes sure not null, goes to next packet if there is one
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
			{
				cout << "Our connection request has been accepted.\n";
			}
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
			case ID_CONNECTION_ATTEMPT_FAILED:
				cout << "Could not connect.\n";
				break;
			case ID_MESSAGE_SENT:
			{
				UserMessage message;
				BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(MessageID));
				//bsIn.Read((char*)&message, sizeof(message));
				bsIn.Read(message.userName);
				bsIn.Read(message.message);
				bsIn.Read(message.timeStamp);
				cout << message.userName << ": " << message.message << endl;

				BitStream bsOut;
				bsOut.Write((MessageID)ID_MESSAGE_RECEIVED);
				//bsOut.Write((char*)&message, sizeof(message));
				bsOut.Write(message.userName);
				bsOut.Write(message.message);
				bsOut.Write(message.timeStamp);
				peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true);	//packet->systemAddress	
				break;
			}
			case ID_MESSAGE_RECEIVED:
			{
				UserMessage message;
				BitStream bsIn(packet->data, packet->length, false);
				bsIn.IgnoreBytes(sizeof(MessageID));
				//bsIn.Read((char*)&message, sizeof(message)); //WHY DOESN'T THIS WORK
				bsIn.Read(message.userName);
				bsIn.Read(message.message);
				bsIn.Read(message.timeStamp);
				cout << message.userName << ": " << message.message << endl;
			}
			break;
			default:
				cout << "Message with identifier " << (int)(packet->data[0]) << " has arrived.\n";
				break;
			}
		}
	}

	RakPeerInterface::DestroyInstance(peer);
	return 0;
}