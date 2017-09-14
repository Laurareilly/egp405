#include "Game.h"
#include <time.h>
#include <chrono>


Game::Game()
{
	mShouldExit = false;
}

Game::~Game()
{
}

void Game::beginLoop()
{
	mElapsedTime = 0.0f;
	auto mStartTime = std::chrono::high_resolution_clock::now();
}

bool Game::endLoop()
{
	sleepUntilNextFrame();
	return mShouldExit;
}

void Game::sleepUntilNextFrame()
{
	double currentTime, lastTime;

	double timeToSleep = LOOP_TARGET_TIME - (mStartTime - currentTime);

	while(timeToSleep > 0)
	{
		lastTime = currentTime;
		double timeElapsed = LOOP_TARGET_TIME - (mStartTime - currentTime);
		timeToSleep -= mElapsedTime;	
	}

}


void Game::processLoop()
{
	ApplicationState theState[1]; //both a pointer and object at the same time, we have the address to it by default
	theState->running = 1;

	while (theState->running)
	{
		updateInput(theState->key);

		updateNetworking(theState);

		updateState(theState);

		render(theState);

	}
}

void Game::updateInput(KeyboardState * keyState)
{
	//-- get mouse, keyboard, controller state
	//-- track this for later

	//--example: in a chat room...
	GetKeyboardState(keyState->key);
}

void Game::updateNetworking(ApplicationState * state)
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

void Game::updateState(ApplicationState * state)
{
	//-- account for all previous updates here
	//-- change application with respect to time and other updates

	//-- e.g chat room
	//-- update chat stream
}

void Game::render(const ApplicationState * state)
{
	//-- "clear" buffer
	//-- display state to display medium

	//e.g chat room
	//-- display chat stream (still rendering)
}
