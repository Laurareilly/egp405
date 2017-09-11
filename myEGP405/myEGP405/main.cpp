<<<<<<< HEAD
#include <stdio.h>
#include "Raknet/RakPeerInterface.h"
#include "Raknet/MessageIdentifiers.h"
#include <string.h>
#include "Raknet/BitStream.h"
#include "Raknet/RakNetTypes.h"  // MessageID

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
/*
struct MyGameGreeting
{
	//GENERAL FORMAT OF NETWORKING STRUCT:
	//HEADER
	//DATA
	char messageID; //one byte
	char greetingMessage[31]; //in this package we can have a 30 byte string (at the very end the null terminator)
};
*/

struct UsernameMessage //for ID_USERNAME and ID_NEW_CLIENT_JOIN (don't use polymorphism or inheritance it will explode)
{
	char messageID;
	char username[31];
};

struct ClientNumberMessage //for ID_CLIENT_NUMBER
{
	char messageID;
	unsigned int clientNumber;
};
=======
#include "GameMessage.h"
#include <iostream>
>>>>>>> 742ef3a52e840d04764df8e558a814e9b11be545

using namespace std;

char str[512];
RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
bool isServer;
RakNet::Packet *packet;


int main(void)
{
	//initLobbyState();
	unsigned int maxClients = 10;
	unsigned int serverPort = 1111;

	cout << 
	printf("(C)lient or (S)erver?\n");
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
		printf("Enter server IP or hit enter for 127.0.0.1\n");
		fgets(str, 512, stdin);
		if (str[0] == 0 || str[0] == '\n') {
			strcpy(str, "127.0.0.1");
		}
		printf("Starting the client.\n");
		peer->Connect(str, serverPort, 0, 0);
	}

	char* name;
	printf("Whats your name?\n");
	fgets(str, 512, stdin);
	name = str;

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

				//client succesfully joins server
			case ID_CONNECTION_REQUEST_ACCEPTED: //the client receives this
			{
				printf("Our connection request has been accepted.\n");

				//set up username packet (using terrible hard-coded values, bad bad bad)
				UsernameMessage username[1] = { ID_USERNAME,  name};
				peer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
<<<<<<< HEAD

				/*
				//Method 2: pack using structs
=======
				
>>>>>>> 742ef3a52e840d04764df8e558a814e9b11be545
				//MyGameGreeting greet = { ID_GAME_MESSAGE_1, "hello struct whop whop"};
				//peer->Send((char*)(&greet), sizeof(greet), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false); //packet->systemaddress tells us who the message came from
			}
				break;
			case ID_USERNAME:
			{
				//we are server, store username in dictionary
				//let everyone know who just joined

				UsernameMessage *username = (UsernameMessage*)packet->data;
				username->messageID = ID_NEW_CLIENT_JOIN;
				peer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true); //true because 

				//send new client their identifier
				ClientNumberMessage clientNumber[1] = {ID_CLIENT_NUMBER, 0};
				//send
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
			//case ID_CHAT_MESSAGE: //server receives this, AND THE CLIENT THEY BOTH DO!!!
			//{
<<<<<<< HEAD
			//	/*
			//	//Method 2: receive using struct
			//	//the data in the packet is already a char*
			//	MyGameGreeting *greet = (MyGameGreeting*)(packet->data);
			//	printf("\n %s \n", greet->greetingMessage);
			//	*/
=======
			//	ChatMessage *message = (ChatMessage*)(packet->data);
			//	printf("\n %s \n", message->userMessage);
>>>>>>> 742ef3a52e840d04764df8e558a814e9b11be545
			//}
			//break;
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

void initLobbyState()
{

}