#include <stdio.h>
#include "Raknet/RakPeerInterface.h"
#include "Raknet/MessageIdentifiers.h"
#include <string.h>
#include "Raknet/BitStream.h"
#include "Raknet/RakNetTypes.h"  // MessageID

enum GameMessages
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1 //End of the raknet reserve headers so now we can make our own
};


//custom data structure
//packaged in a bit aligned format
//data structures that have 1 byte declared by default get padded
//the very first byte in the stream is the data header and the rest is the message!
#pragma pack (push, 1)
struct MyGameGreeting
{
	//GENERAL FORMAT OF NETWORKING STRUCT:
	//HEADER
	//DATA
	char messageID; //one byte
	char greetingMessage[31]; //in this package we can have a 30 byte string (at the very end the null terminator)
};
#pragma pack (pop)


int main(void)
{
	char str[512];
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer;
	RakNet::Packet *packet;

	unsigned int maxClients = 10;
	unsigned int serverPort = 1111;

	printf("(C) or (S)erver?\n");
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
			case ID_CONNECTION_REQUEST_ACCEPTED: //the client receives this
			{
				printf("Our connection request has been accepted.\n");

				// Use a BitStream to write a custom user message
				// Bitstreams are easier to use than sending casted structures, and handle endian swapping automatically

				//Method 1: pack using bitstream
				//RakNet::BitStream bsOut;
				//bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);
				//bsOut.Write("Hello world from client");
				////the send command basically tells raknet how to configure the lower layers
				////high priority and reliable ordered describe the transport layer- how the data should be transported
				////system address flag describes the internet layer: ip address
				////which protocol are we using on the transport layer? UDP....but we are emulating TCP thats why it says high_priority
				////this is asynch echange and it's fine for now
				//peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);


				//Method 2: pack using structs
				MyGameGreeting greet = { ID_GAME_MESSAGE_1, "hello struct whop whop"};
				peer->Send((char*)(&greet), sizeof(greet), HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);
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
			case ID_GAME_MESSAGE_1: //server receives this, AND THE CLIENT THEY BOTH DO!!!
			{
				////Method 1: unpack using bitstream
				//RakNet::RakString rs;
				//RakNet::BitStream bsIn(packet->data, packet->length, false);
				//bsIn.IgnoreBytes(sizeof(RakNet::MessageID));
				//bsIn.Read(rs);
				//printf("%s\n", rs.C_String());


				//RakNet::BitStream bsOut;
				//bsOut.Write((RakNet::MessageID)ID_GAME_MESSAGE_1);

				//if(isServer)
				//	bsOut.Write("Hello world from server");
				//else
				//	bsOut.Write("Hello world from client");

				//peer->Send(&bsOut, HIGH_PRIORITY, RELIABLE_ORDERED, 0, packet->systemAddress, false);

				//Method 2: receive using struct
				//the data in the packet is already a char*
				MyGameGreeting *greet = (MyGameGreeting*)(packet->data);
				printf("\n %s \n", greet->greetingMessage);
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