#include <stdio.h>
#include "Raknet/RakPeerInterface.h"
#include "Raknet/MessageIdentifiers.h"
#include <string.h>
#include "Raknet/BitStream.h"
#include "Raknet/RakNetTypes.h"  // MessageID
#include "GameMessage.h"
#include <iostream>
#include <string>
#include <Windows.h>

using namespace std;

enum GameMessages
{
	ID_GAME_MESSAGES = ID_USER_PACKET_ENUM,

	//handshake exchange
	ID_USERNAME,				//client responds to connection by sending it's username to server
	ID_NEW_CLIENT_JOIN,			//server broadcasts welcome message to all clients
	ID_CLIENT_NUMBER,			//server associates username with client number

	//message exchange
	ID_CHAT_MESSAGE,			//send by anyone

	//misc.
	ID_SEND_ALL,				//sent by client

	ID_CLIENT_TO_SERVER,		//Clients exclusively send this, but will never receive and interpret it
	ID_RECEIVE_MESSAGE,			//Server received ID_CtS, and determined that the message isn't a DM. This is then sent by server, and interpreted by clients
	ID_RECEIVE_DIRECT_MESSAGE,	//Server received ID_CtS, and determined it was a DM. This is sent back to the sender, and is sent to the recipient of the DM (PURPLE)
	ID_SERVER_MESSAGE			//Sent by Server, received by Clients, displayed in all red
};

enum RoomState
{
	LOBBY = 0,
	CHATROOM
};

//custom data structure
//no padding
//packaged in a bit aligned format
//data structures that have 1 byte declared by default get padded
//the very first byte in the stream is the data header and the rest is the message!
#pragma pack (push, 1)

struct UsernameMessage //for ID_USERNAME and ID_NEW_CLIENT_JOIN (don't use polymorphism or inheritance it will explode)
{
	char messageID;
	char username[31];
	char message[96];
	//RakNet::SystemAddress mContactInfo;
};

struct ClientNumberMessage //for ID_CLIENT_NUMBER
{
	char messageID;
	unsigned int clientNumber;
};

#pragma pack(pop)

char str[512];
RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
bool isServer;
RakNet::Packet *packet;

unsigned int maxClients = 10;
unsigned int serverPort = 1111;

UsernameMessage connectedUserList[10];
int connectedUserCount = 10;


//preset colors for specific messages
//char red[] = {}
//char purple[]
//char black[]
//char white[]
char blue[] = { 0x1b, '[', '1', ';', '3', '4', 'm', 0 };

int gDefaultColors;

//http://www.cplusplus.com/forum/beginner/1640/
WORD GetConsoleTextAttribute(HANDLE hCon)
{
	CONSOLE_SCREEN_BUFFER_INFO con_info;
	GetConsoleScreenBufferInfo(hCon, &con_info);
	return con_info.wAttributes;
}

void SetTextRed()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
}

void SetTextPurple()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
}

void SetTextDefault()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), gDefaultColors);
}

int main(void)
{
	gDefaultColors = GetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE));
	char myUsernameString[31];

	SetTextPurple();
	cout << "Welcome. Enter a username.\n";
	SetTextDefault();

	fgets(myUsernameString, 31, stdin);

	cout << "Would you like to be a (S)erver or a (C)lient?\n";
	fgets(str, 512, stdin);

	if ((str[0] == 'c') || (str[0] == 'C'))
	{
		RakNet::SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		isServer = false;
	}
	else {
		RakNet::SocketDescriptor sd(serverPort, 0);
		peer->Startup(maxClients, &sd, 1);
		isServer = true;
	}

	if (isServer)
	{
		printf("Starting the server.\n");
		// We need to let the server accept incoming connections from the clients
		peer->SetMaximumIncomingConnections(maxClients);
	}
	else {
		system("Color 1A");
		printf("Enter server IP or hit enter for 127.0.0.1\n");
		fgets(str, 512, stdin);
		if (str[0] == 0 || str[0] == '\n') {
			strcpy(str, "127.0.0.1");
		}
		printf("Entering chatroom.\n");
		peer->Connect(str, serverPort, 0, 0);
	}

	while (1) //this loop SUCKS because we (the users) are confused on who is sending/receiving each message, processing cases unnecessarily
	{
		for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), packet = peer->Receive())
		{
			switch (packet->data[0])
			{
			case ID_REMOTE_DISCONNECTION_NOTIFICATION:
				printf("Another client has disconnected.\n");
				break;
			case ID_REMOTE_CONNECTION_LOST:
				printf("Another client has lost the connection.\n");
				break;
			case ID_REMOTE_NEW_INCOMING_CONNECTION:
				printf("Another client has connected.\n");
				break;

			case ID_USERNAME:
			{
				int i;
				UsernameMessage *username = (UsernameMessage*)packet->data;
				for (i = 0; i < connectedUserCount; ++i)
				{
					if (connectedUserList[i].messageID == 0)
					{
						//we are server, store username in dictionary
						connectedUserList[i] = *username;
						break;
					}
				}
				printf(username->username);
				UsernameMessage newMessage[1] = { ID_NEW_CLIENT_JOIN , "silas", "heck no" };
				strcpy(newMessage[0].username, username->username);
				strcpy(newMessage[0].message, username->message);

				//let everyone know who just joined
				username->messageID = ID_NEW_CLIENT_JOIN;
				//strcpy(username->username, username->username);

				RakNet::SystemAddress desiredUserAddress;
				desiredUserAddress = packet->systemAddress;

				peer->Send((char *)newMessage, sizeof(newMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false); //false sends it back to the person
				//peer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true); //true because going to be sent to everyone

				//send new client their identifier
				ClientNumberMessage clientNumber[1] = { ID_CLIENT_NUMBER, i }; //sending the client number grabbed from the forloop
				//send
				peer->Send((char*)clientNumber, sizeof(clientNumber), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false); //
			}
			break;

			case ID_NEW_CLIENT_JOIN:
			{
				UsernameMessage *username = (UsernameMessage*)packet->data;
				//printf(*username->username.c_str());
				printf("Username: %s has joined.\n", username->username);

				UsernameMessage newMessage[1] = { ID_CHAT_MESSAGE , "myname", "supergoodmessage" };
				peer->Send((char*)newMessage, sizeof(newMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			}
				break;

				//client succesfully joins server
			case ID_CONNECTION_REQUEST_ACCEPTED: //the client receives this
			{
				printf("Our connection request has been accepted.\n");

				//set up username packet (using terrible hard-coded values, bad bad bad)
				UsernameMessage username[1] = { ID_USERNAME,  "", "hello" };
				//for (int index = 0; index < 31; index++)
				//	username[0].username[index] = myUsernameString[index];
				strcpy(username[0].username, myUsernameString);
				peer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			}
				break;
				/*
				//Method 2: pack using structs

				//MyGameGreeting greet = { ID_GAME_MESSAGE_1, "hello struct whop whop"};
				//peer->Send((char*)(&greet), sizeof(greet), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false); //packet->systemaddress tells us who the message came from
			}
				break;
			case ID_USERNAME:
			{
				we are server, store username in dictionary
				let everyone know who just joined

				UsernameMessage *username = (UsernameMessage*)packet->data;
				username->messageID = ID_NEW_CLIENT_JOIN;
				peer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true); //true because 

				send new client their identifier
				ClientNumberMessage clientNumber[1] = {ID_CLIENT_NUMBER, 0};
				send
				peer->Send((char*)clientNumber, sizeof(clientNumber), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true); //wait so should this be true or false
			}
				break;
			case ID_NEW_INCOMING_CONNECTION:
				printf("A connection is incoming.\n");
				break;
			case ID_NO_FREE_INCOMING_CONNECTIONS:
				printf("The server is full.\n");
				break;
			case ID_DISCONNECTION_NOTIFICATION:
				if (isServer) {
					printf("A client has disconnected.\n");
				}
				else {
					printf("We have been disconnected.\n");
				}
				break;
			case ID_CONNECTION_LOST:
				if (isServer) {
					printf("A client lost the connection.\n");
				}
				else {
					printf("Connection lost.\n");
				}
				break;
				*/
			case ID_CHAT_MESSAGE: //server receives this, AND THE CLIENT THEY BOTH DO!!!
			{
				UsernameMessage *message = (UsernameMessage*)(packet->data);
				printf("\n %s \n", message->username);
			}
			break;
			
			default:
				printf("Message with identifier %i has arrived.\n", packet->data[0]);
				break;
			}
		}
	}


	// TODO - Add code body here

	RakNet::RakPeerInterface::DestroyInstance(peer);
	return 0;
}