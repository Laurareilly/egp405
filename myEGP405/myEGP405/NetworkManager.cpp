#include "NetworkManager.h"
#include "GameMessage.h"
#include "Raknet/MessageIdentifiers.h"
#include <stdio.h>

NetworkManager::NetworkManager()
{
	mpPeer = RakPeerInterface::GetInstance();
}

NetworkManager::~NetworkManager()
{
	RakPeerInterface::DestroyInstance(mpPeer);
}

void NetworkManager::initializeNetwork()
{
	//cout << "Welcome. Enter a username.\n";
	//SetTextDefault();

	//fgets(myUsernameString, 31, stdin);

	//cout << "Would you like to be a (S)erver or a (C)lient?\n";
	//fgets(str, 512, stdin);
	

	//call either init server or init client

}

bool NetworkManager::initServer(int cPort)
{
	bool serverSuccess = false;
	maxClients = 20;
	serverPort = cPort;
	sd = new SocketDescriptor(serverPort, 0);
	serverSuccess =	mpPeer->Startup(maxClients, sd, 1);
	mpPeer->SetMaximumIncomingConnections(maxClients);
	mIsServer = true;

	return serverSuccess;
}

bool NetworkManager::initClient(int cPort, char* cIP)
{	
	sd = new SocketDescriptor();
	mpPeer->Startup(1, sd, 1);
	serverPort = cPort;

	//need to ask for the server ip before this?
	mpPeer->Connect(cIP, serverPort, 0, 0);

	mIsServer = false;
	return true;

}

void NetworkManager::updateServer()
{
	for (mpPacket = mpPeer->Receive(); mpPacket; mpPeer->DeallocatePacket(mpPacket), mpPeer->Receive())
	{
		switch (mpPacket->data[0])
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			//printf("Another client has disconnected.\n");
			break;
		case ID_REMOTE_CONNECTION_LOST:
			//printf("Another client has lost the connection.\n");
			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			//printf("Another client has connected.\n");
			break;

			//	case ID_USERNAME:
			//	{
			//		//int i;
			//		//UsernameMessage *username = (UsernameMessage*)packet->data;
			//		//for (i = 0; i < connectedUserCount; ++i)
			//		//{
			//		//	if (connectedUserList[i].messageID == 0)
			//		//	{
			//		//		//we are server, store username in dictionary
			//		//		connectedUserList[i] = *username;
			//		//		break;
			//		//	}
			//		//}
			//		//printf(username->username);
			//		//UsernameMessage newMessage[1] = { ID_NEW_CLIENT_JOIN , "silas", "heck no" };
			//		//strcpy(newMessage[0].username, username->username);
			//		//strcpy(newMessage[0].message, username->message);

			//		////let everyone know who just joined
			//		//username->messageID = ID_NEW_CLIENT_JOIN;
			//		////strcpy(username->username, username->username);

			//		//RakNet::SystemAddress desiredUserAddress;
			//		//desiredUserAddress = packet->systemAddress;

			//		//peer->Send((char *)newMessage, sizeof(newMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false); //false sends it back to the person
			//		////peer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true); //true because going to be sent to everyone

			//		////send new client their identifier
			//		//ClientNumberMessage clientNumber[1] = { ID_CLIENT_NUMBER, i }; //sending the client number grabbed from the forloop
			//		////send
			//		//peer->Send((char*)clientNumber, sizeof(clientNumber), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false); //
			//	}
			//	break;

			//	case ID_NEW_CLIENT_JOIN:
			//	{
			//		UsernameMessage *username = (UsernameMessage*)packet->data;
			//		//printf(*username->username.c_str());
			//		printf("Username: %s has joined.\n", username->username);

			//		UsernameMessage newMessage[1] = { ID_CHAT_MESSAGE , "myname", "supergoodmessage" };
			//		peer->Send((char*)newMessage, sizeof(newMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
			//	}
			//		break;

			//		//client succesfully joins server
				case ID_CONNECTION_REQUEST_ACCEPTED: //the client receives this
				{
					//printf("Our connection request has been accepted.\n");

					////set up username packet (using terrible hard-coded values, bad bad bad)
					//UsernameMessage username[1] = { ID_USERNAME,  "", "hello" };
					////for (int index = 0; index < 31; index++)
					////	username[0].username[index] = myUsernameString[index];
					//strcpy(username[0].username, myUsernameString);
					//peer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
				}
					break;
					/*
					//Method 2: pack using structs

					//MyGameGreeting greet = { ID_GAME_MESSAGE_1, "hello struct whop whop"};
					//peer->Send((char*)(&greet), sizeof(greet), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false); //packet->systemaddress tells us who the message came from
				}
					break;
			//	case ID_USERNAME:
			//	{
			//		we are server, store username in dictionary
			//		let everyone know who just joined

			//		UsernameMessage *username = (UsernameMessage*)packet->data;
			//		username->messageID = ID_NEW_CLIENT_JOIN;
			//		peer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true); //true because 

			//		send new client their identifier
			//		ClientNumberMessage clientNumber[1] = {ID_CLIENT_NUMBER, 0};
			//		send
			//		peer->Send((char*)clientNumber, sizeof(clientNumber), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true); //wait so should this be true or false
			//	}
			//		break;
			//	case ID_NEW_INCOMING_CONNECTION:
			//		printf("A connection is incoming.\n");
			//		break;
			//	case ID_NO_FREE_INCOMING_CONNECTIONS:
			//		printf("The server is full.\n");
			//		break;
			//	case ID_DISCONNECTION_NOTIFICATION:
			//		if (isServer) {
			//			printf("A client has disconnected.\n");
			//		}
			//		else {
			//			printf("We have been disconnected.\n");
			//		}
			//		break;
			//	case ID_CONNECTION_LOST:
			//		if (isServer) {
			//			printf("A client lost the connection.\n");
			//		}
			//		else {
			//			printf("Connection lost.\n");
			//		}
			//		break;
			//		*/
			//	case ID_CHAT_MESSAGE: //server receives this, AND THE CLIENT THEY BOTH DO!!!
			//	{
			//		UsernameMessage *message = (UsernameMessage*)(packet->data);
			//		printf("\n %s \n", message->username);
			//	}
			//	break;
			//	
			default:
			{
				printf("aifjai");
			}
				break;
				//printf("Message with identifier %i has arrived.\n", packet->data[0]);
			}

	}
}
//
//void NetworkManager::updateClient()
//{
//	for (mpPacket = mpPeer->Receive(); mpPacket; mpPeer->DeallocatePacket(mpPacket), mpPeer->Receive())
//	{
//		switch (mpPacket->data[0])
//		{
//		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
//			//printf("Another client has disconnected.\n");
//			break;
//		case ID_REMOTE_CONNECTION_LOST:
//			//printf("Another client has lost the connection.\n");
//			break;
//		case ID_REMOTE_NEW_INCOMING_CONNECTION:
//			//printf("Another client has connected.\n");
//			break;
//
//			//	case ID_USERNAME:
//			//	{
//			//		//int i;
//			//		//UsernameMessage *username = (UsernameMessage*)packet->data;
//			//		//for (i = 0; i < connectedUserCount; ++i)
//			//		//{
//			//		//	if (connectedUserList[i].messageID == 0)
//			//		//	{
//			//		//		//we are server, store username in dictionary
//			//		//		connectedUserList[i] = *username;
//			//		//		break;
//			//		//	}
//			//		//}
//			//		//printf(username->username);
//			//		//UsernameMessage newMessage[1] = { ID_NEW_CLIENT_JOIN , "silas", "heck no" };
//			//		//strcpy(newMessage[0].username, username->username);
//			//		//strcpy(newMessage[0].message, username->message);
//
//			//		////let everyone know who just joined
//			//		//username->messageID = ID_NEW_CLIENT_JOIN;
//			//		////strcpy(username->username, username->username);
//
//			//		//RakNet::SystemAddress desiredUserAddress;
//			//		//desiredUserAddress = packet->systemAddress;
//
//			//		//peer->Send((char *)newMessage, sizeof(newMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false); //false sends it back to the person
//			//		////peer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, RakNet::UNASSIGNED_SYSTEM_ADDRESS, true); //true because going to be sent to everyone
//
//			//		////send new client their identifier
//			//		//ClientNumberMessage clientNumber[1] = { ID_CLIENT_NUMBER, i }; //sending the client number grabbed from the forloop
//			//		////send
//			//		//peer->Send((char*)clientNumber, sizeof(clientNumber), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false); //
//			//	}
//			//	break;
//
//			//	case ID_NEW_CLIENT_JOIN:
//			//	{
//			//		UsernameMessage *username = (UsernameMessage*)packet->data;
//			//		//printf(*username->username.c_str());
//			//		printf("Username: %s has joined.\n", username->username);
//
//			//		UsernameMessage newMessage[1] = { ID_CHAT_MESSAGE , "myname", "supergoodmessage" };
//			//		peer->Send((char*)newMessage, sizeof(newMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
//			//	}
//			//		break;
//
//			//		//client succesfully joins server
//			//	case ID_CONNECTION_REQUEST_ACCEPTED: //the client receives this
//			//	{
//			//		printf("Our connection request has been accepted.\n");
//
//			//		//set up username packet (using terrible hard-coded values, bad bad bad)
//			//		UsernameMessage username[1] = { ID_USERNAME,  "", "hello" };
//			//		//for (int index = 0; index < 31; index++)
//			//		//	username[0].username[index] = myUsernameString[index];
//			//		strcpy(username[0].username, myUsernameString);
//			//		peer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
//			//	}
//			//		break;
//			//		/*
//			//		//Method 2: pack using structs
//
//			//		//MyGameGreeting greet = { ID_GAME_MESSAGE_1, "hello struct whop whop"};
//			//		//peer->Send((char*)(&greet), sizeof(greet), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false); //packet->systemaddress tells us who the message came from
//			//	}
//			//		break;
//			//	case ID_USERNAME:
//			//	{
//			//		we are server, store username in dictionary
//			//		let everyone know who just joined
//
//			//		UsernameMessage *username = (UsernameMessage*)packet->data;
//			//		username->messageID = ID_NEW_CLIENT_JOIN;
//			//		peer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true); //true because 
//
//			//		send new client their identifier
//			//		ClientNumberMessage clientNumber[1] = {ID_CLIENT_NUMBER, 0};
//			//		send
//			//		peer->Send((char*)clientNumber, sizeof(clientNumber), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, true); //wait so should this be true or false
//			//	}
//			//		break;
//			//	case ID_NEW_INCOMING_CONNECTION:
//			//		printf("A connection is incoming.\n");
//			//		break;
//			//	case ID_NO_FREE_INCOMING_CONNECTIONS:
//			//		printf("The server is full.\n");
//			//		break;
//			//	case ID_DISCONNECTION_NOTIFICATION:
//			//		if (isServer) {
//			//			printf("A client has disconnected.\n");
//			//		}
//			//		else {
//			//			printf("We have been disconnected.\n");
//			//		}
//			//		break;
//			//	case ID_CONNECTION_LOST:
//			//		if (isServer) {
//			//			printf("A client lost the connection.\n");
//			//		}
//			//		else {
//			//			printf("Connection lost.\n");
//			//		}
//			//		break;
//			//		*/
//			//	case ID_CHAT_MESSAGE: //server receives this, AND THE CLIENT THEY BOTH DO!!!
//			//	{
//			//		UsernameMessage *message = (UsernameMessage*)(packet->data);
//			//		printf("\n %s \n", message->username);
//			//	}
//			//	break;
//			//	
//		default:
//			//printf("Message with identifier %i has arrived.\n", packet->data[0]);
//			break;
//		}
//
//	}
//}
