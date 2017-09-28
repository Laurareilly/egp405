#pragma once

/*
Project 1
EGP 405-02
Laura Reilly -- 0972157
Robert Mitchell -- 0935286

We certify that this work is entirely our own.
The assessor of this project may reproduce this
project and provide copies to other academic staff,
and/or communicate a copy of this project to a plagiarism
- checking service, which may retain a copy of the project on its database.�
*/

#include "RakNet\RakPeerInterface.h"
#include "RakNet\RakNetTypes.h"
#include "RakNet\MessageIdentifiers.h"
#include <string>

using namespace RakNet;

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();

	void initializeNetwork();

	bool initServer(int cPort);
	void initClient(int cPort, char* cIP);
	void updateServer();
	void ShutdownServer();

	void SendNetworkedMessage(char* cMessage, int cSenderID);
	void SendNetworkedMove(int cMoveSlot);
	void DisconnectFromPeers();

	//void updateClient();

	RakPeerInterface *mpPeer;
	Packet *mpPacket;
	SocketDescriptor *sd;
	bool mIsServer;


	enum GameMessages
	{
		ID_GAME_MESSAGES = ID_USER_PACKET_ENUM + 1,       //ID_USER_PACKET_ENUM,

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
		ID_SERVER_MESSAGE,			//Sent by Server, received by Clients, displayed in all red
		ID_PEER_LEAVE,				//Sent by EITHER ***PEER*** when they go to the lobby or press SHIFT ESC

		ID_SEND_MOVE, //send by either peer, contains int that corresponds to slot in table
		ID_RECEIVE_TURN_NUMBER,
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

	struct ClientToServerMessage
	{
		char messageID;
		unsigned int clientID;
		char message[96];
	};

	struct ClientNumberMessage //for ID_CLIENT_NUMBER
	{
		char messageID;
		unsigned int clientNumber;
	};

#pragma pack(pop)

private:


	unsigned int maxClients = 1;
	unsigned int serverPort = 5546;
};