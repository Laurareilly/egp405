/*
This file was created by Laura Reilly using tutorials from http://www.jenkinssoftware.com/raknet/manual/tutorialsample3.html
I certify that this work is entirely my own.
The assessor of this project may reproduce this project
and provide copies to other academic staff, and/or communicate
a copy of this project to a plagiarism-checking service,
which may retain a copy of the project on its database.”
*/

#pragma once
#include "../../../DevSDKs/DevSDKs/include/RakNet/RakPeerInterface.h" //this needs to be fixed
#include "../../../DevSDKs/DevSDKs/include/RakNet/MessageIdentifiers.h" //so does this
#include "../../../DevSDKs/DevSDKs/include/RakNet/RakNetTypes.h"
#include "../../../DevSDKs/DevSDKs/include/RakNet/Bitstream.h"

using namespace RakNet;

enum GameMessage
{
	ID_GAME_MESSAGE_1 = ID_USER_PACKET_ENUM + 1
};