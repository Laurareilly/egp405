//INTRO TO NETWORKED ARCHITECTURE - ASYNCH CHAT ROOM APPLICATION

#include <Windows.h>
#include "RakNet\RakPeerInterface.h"


struct KeyboardState
{
	unsigned char key[256];
};


struct ApplicationState
{
	int running;

	KeyboardState key[1];
};



void updateInput(KeyboardState* keyState)
{
	//-- get mouse, keyboard, controller state
	//-- track this for later

	//--example: in a chat room...
	GetKeyboardState(keyState->key);
}

void updateNetworking(/*param*/)
{
	//-- TMP: make some networking state 
	//that stores a peer instance
	//fow now a temp
	RakNet::RakPeerInterface *peer;

	//-- receive packets until no more packets
	//-- while packet exists
	//  --process message
	//  --track data (if any) for later
	RakNet::Packet *packet;
	for (packet = peer->Receive(); packet; peer->DeallocatePacket(packet), peer->Receive())
	{
		//HOOREAY NETWORKING MESSAGES
		//dont print anything unless debugging


	}
}

void updateState(ApplicationState* state)
{
	//-- account for all previous updates here
	//-- change application with respect to time and other updates

	//-- e.g chat room
	//-- update chat stream
}

void render(const ApplicationState* state)
{
	//-- "clear" buffer
	//-- display state to display medium

	//e.g chat room
	//-- display chat stream (still rendering)
}


int processLoop()
{
	ApplicationState theState[1]; //both a pointer and object at the same time, we have the address to it by default
	theState->running = 1;

	while (theState->running)
	{
		updateInput(theState->key);

		updateNetworking(/*i think this needs to be appstate*/);

		updateState(theState);

		render(theState);
	}
}