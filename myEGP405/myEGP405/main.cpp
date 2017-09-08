#include "GameMessage.h"
#include <iostream>

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
			//	ChatMessage *message = (ChatMessage*)(packet->data);
			//	printf("\n %s \n", message->userMessage);
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