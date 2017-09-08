#pragma once

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



//custom data structure
//no padding
//packaged in a bit aligned format
//data structures that have 1 byte declared by default get padded
//the very first byte in the stream is the data header and the rest is the message!
#pragma pack (push, 1)

//GENERAL FORMAT OF NETWORKING STRUCT:
//HEADER
//DATA
struct UsernameMessage //for ID_USERNAME and ID_NEW_CLIENT_JOIN (don't use polymorphism or inheritance it will explode)
{
	char messageID;//one byte
	char username[31];
};

struct ClientNumberMessage //for ID_CLIENT_NUMBER
{
	char messageID;//one byte
	unsigned int clientNumber;
};

struct ChatMessage //for ID_CHAT_MESSAGE and maybe ID_SEND_ALL
{
	char messageID; //one byte
	char userMessage[31]; //in this package we can have a 30 byte string (at the very end the null terminator)
};
#pragma pack (pop)