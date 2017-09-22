#include "NetworkManager.h"
#include "GameMessage.h"
#include "Raknet/MessageIdentifiers.h"
#include <stdio.h>
#include "Game.h"
#include "commonFile.h"
#include "ApplicationState.h"

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
	mpPeer->SetMaximumIncomingConnections(maxClients);
	serverSuccess =	mpPeer->Startup(maxClients, sd, 1);
	mIsServer = true;

	return serverSuccess;
}

void NetworkManager::initClient(int cPort, char* cIP)
{	
	mIsServer = false;
	sd = new SocketDescriptor();
	mpPeer->Startup(1, sd, 1);

	if (!mpPeer)
	{
		printf("Peer is false");
	}

	serverPort = cPort;

	//need to ask for the server ip before this?
	mpPeer->Connect(cIP, serverPort, 0, 0);

}

void NetworkManager::updateServer()
{
	for (mpPacket = mpPeer->Receive(); mpPacket; mpPeer->DeallocatePacket(mpPacket), mpPacket = mpPeer->Receive())
	{
		if ((int)(mpPacket->data[0] > 144)) //For some reason client -> connect's packet hits accept, then hits enum 221, then throws an exception
		{
			break;
		}
		switch (mpPacket->data[0])
		{
		case ID_REMOTE_DISCONNECTION_NOTIFICATION:
			//printf("Another client has disconnected.\n");
			break;
		case ID_REMOTE_CONNECTION_LOST:
			//printf("Another client has lost the connection.\n");

			//what I would like to do here is send a message to all users
			//they would receive it and send back a message with their clientID (in the background, they don't know about this)
			//I'd check this against the list of clientIDs, find the one that is missing, and
				//remove the info for that one
			//This is mostly a time constraint because the fundamentals of the project took a long ass time to fix

			break;
		case ID_REMOTE_NEW_INCOMING_CONNECTION:
			//printf("Another client has connected.\n");
			break;

			case ID_NEW_CLIENT_JOIN:
			{
				UsernameMessage *username = (UsernameMessage*)mpPacket->data;

				gpGame->theState->ReceiveMessage(username->username, " has joined!");
			}
				break;

			//		//client succesfully joins server
				case ID_CONNECTION_REQUEST_ACCEPTED: //the client receives this
				{
					printf("Our connection request has been accepted.\n");
					gpGame->theState->AcceptedToServer();


					UsernameMessage username[1] = { ID_USERNAME,  "", "hello" };

					gpGame->theState->SetSystemAddress(mpPacket->systemAddress);
					strcpy(username[0].username, gpGame->theState->getUsername());
					mpPeer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, mpPacket->systemAddress, false);

					//gpGame->theState->isServer = false;
				}
					break;
				case ID_USERNAME:
				{
					/*we are server, store username in dictionary
					let everyone know who just joined*/

					//gpGame->theState->AcceptedToServer();


					UsernameMessage *username = (UsernameMessage*)mpPacket->data;
					username->messageID = ID_NEW_CLIENT_JOIN;
					mpPeer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, mpPacket->systemAddress, true); //true because 
					gpGame->theState->ReceiveMessage(username->username, " has joined!");

					int clientIDNum = gpGame->theState->getNextOpenUsernameIndex();
					char newUsername[31];
					strncpy(newUsername, username->username, 31);

					gpGame->theState->allSystemAddresses[clientIDNum] = mpPacket->systemAddress;

					gpGame->theState->insertUsernameIntoList(username->username, clientIDNum);

					//send new client their identifier
					ClientNumberMessage clientNumber[1] = {ID_CLIENT_NUMBER, clientIDNum};
					//send
					mpPeer->Send((char*)clientNumber, sizeof(clientNumber), HIGH_PRIORITY, RELIABLE_ORDERED, 0, mpPacket->systemAddress, false); //wait so should this be true or false
				}
					break;
				case ID_CLIENT_NUMBER:
				{
					ClientNumberMessage *clientNumber = (ClientNumberMessage*)mpPacket->data;
					gpGame->theState->SetClientID(clientNumber->clientNumber);
					//gpGame->theState->SetClientID(2);
					//verified that we are setting SetClientID to 1!
					//if (clientNumber->clientNumber == 1)
					//	gpGame->theState->SetClientID(1);
				}
				break;
				case ID_CLIENT_TO_SERVER:
				{
					ClientToServerMessage *newMessage = (ClientToServerMessage*)mpPacket->data;
					std::string receivedMessage = newMessage->message;
					bool messageIsGood = false;
					int spaceIndex = 0;
					if (receivedMessage[0] == '@') //private message!
					{
						receivedMessage.erase(0, 1);
						for (int i = 1; i < receivedMessage.length(); i++)
						{
							if (receivedMessage[i] == ' ')
							{
								messageIsGood = true;
								spaceIndex = i;
								break;
							}
						}

						std::string usernameString = receivedMessage;
						usernameString.erase(spaceIndex, usernameString.length() - spaceIndex);
						receivedMessage.erase(0, spaceIndex + 1);

						
						int targetIndex = -1;
						for (int i = 0; i < 20; i++)
						{
							if (usernameString == gpGame->theState->getUsernameList()[i])
							{
								targetIndex = i;
							}
						}

						std::string tempUsernameThing = gpGame->theState->getUsernameList()[newMessage->clientID];
						std::string recipientUsernameString = "@From " + tempUsernameThing;
						std::string backToSenderUsernameString = "@To " + usernameString;

						UsernameMessage username[1] = { ID_RECEIVE_MESSAGE, "", "hello" };
						//strcpy(username->username, gpGame->theState->getUsernameList()[newMessage->clientID]);


						//send it back
						strcpy(username->username, backToSenderUsernameString.c_str());
						strcpy(username->message, receivedMessage.c_str());
						mpPeer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, mpPacket->systemAddress, false);


						//send it to the desired recipient
						if (targetIndex != -1)
						{
							strcpy(username->username, recipientUsernameString.c_str());
							strcpy(username->message, receivedMessage.c_str());
							mpPeer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gpGame->theState->allSystemAddresses[targetIndex], false);
							gpGame->theState->ReceiveMessage(username->username, username->message);
						}
					}
					else
					{
						UsernameMessage username[1] = { ID_RECEIVE_MESSAGE, "", "hello" };
						strcpy(username->message, newMessage->message);
						strcpy(username->username, gpGame->theState->getUsernameList()[newMessage->clientID]);
						mpPeer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, mpPacket->systemAddress, true);
						mpPeer->Send((char*)username, sizeof(username), HIGH_PRIORITY, RELIABLE_ORDERED, 0, mpPacket->systemAddress, false);
						gpGame->theState->ReceiveMessage(username->username, username->message);
					}
				}
				break;
				case ID_RECEIVE_MESSAGE:
				{
					UsernameMessage *username = (UsernameMessage*)mpPacket->data;
					gpGame->theState->ReceiveMessage(username->username, username->message);
				}
				break;
				case ID_NEW_INCOMING_CONNECTION:
					printf("A connection is incoming.\n");
					break;
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
				printf("Default Constructor Hit\n");
				printf("%f", (int)mpPacket->data[0]);
			}
				break;
				//printf("Message with identifier %i has arrived.\n", packet->data[0]);
			}

	}
}

void NetworkManager::SendNetworkedMessage(char* cMessage, int cSenderID)
{
	//mpPeer->Send((char*)newMessage, sizeof(newMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, mpPacket->systemAddress, false);
	if (mIsServer)
	{
		UsernameMessage newMessage[1] = { ID_RECEIVE_MESSAGE, "" , "hello" };
		strcpy(newMessage[0].username, gpGame->theState->getUsername());
		strcpy(newMessage[0].message, cMessage);
		mpPeer->Send((char*)newMessage, sizeof(newMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, UNASSIGNED_SYSTEM_ADDRESS, true);
	}
	else
	{
		//ID_CLIENT_TO_SERVER
		ClientToServerMessage newMessage[1] = { ID_CLIENT_TO_SERVER,  cSenderID, "hello" };
		strcpy(newMessage[0].message, cMessage);
		mpPeer->Send((char*)newMessage, sizeof(newMessage), HIGH_PRIORITY, RELIABLE_ORDERED, 0, gpGame->theState->GetSystemAddress(), false);
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
