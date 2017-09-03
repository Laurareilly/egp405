/*
This file was created by Laura Reilly (0972157) using tutorials from http://www.jenkinssoftware.com/raknet/manual/tutorialsample3.html
I certify that this work is entirely my own.
The assessor of this project may reproduce this project
and provide copies to other academic staff, and/or communicate
a copy of this project to a plagiarism-checking service,
which may retain a copy of the project on its database.”
*/

#pragma once

#include "RakNet/RakPeerInterface.h"
#include "RakNet/MessageIdentifiers.h"
#include "RakNet/Bitstream.h"
#include "RakNet/RakNetTypes.h"
#include "RakNet/GetTime.h"


#include <string>

using namespace RakNet;
using namespace std;

enum GameMessage
{
	ID_USER_MESSAGE = ID_USER_PACKET_ENUM + 1,
	ID_MESSAGE_SENT,
	ID_MESSAGE_RECEIVED
};

#pragma pack(push, 1)
struct UserMessage
{
	unsigned char typeId = ID_USER_MESSAGE;
	string message;
	string userName;
	Time timeStamp;
	unsigned char useTimeStamp;
};
#pragma pack(pop)
