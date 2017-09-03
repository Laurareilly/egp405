#pragma once

#include "../../../DevSDKs/DevSDKs/include/RakNet/RakPeerInterface.h" //this needs to be fixed
#include "../../../DevSDKs/DevSDKs/include/RakNet/MessageIdentifiers.h" //so does this
#include "../../../DevSDKs/DevSDKs/include/RakNet/RakNetTypes.h"
#include "../../../DevSDKs/DevSDKs/include/RakNet/Bitstream.h"
#include "../../../DevSDKs/DevSDKs/include/RakNet/RakNetTime.h"
#include "../../../DevSDKs/DevSDKs/include/RakNet/GetTime.h"
#include <string>

using namespace RakNet;
using namespace std;

enum GameMessage
{
	ID_USER_MESSAGE = ID_USER_PACKET_ENUM + 1,
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
