#include "Game.h"
#include "Timer.h"
#include "NetworkManager.h"
#include "Lobby.h"
#include "GameLocalState.h"
#include "ApplicationState.h"
#include <stdio.h>

//http://www.cplusplus.com/forum/beginner/1640/
//WORD GetConsoleTextAttribute(HANDLE hCon)
//{
//	CONSOLE_SCREEN_BUFFER_INFO con_info;
//	GetConsoleScreenBufferInfo(hCon, &con_info);
//	return con_info.wAttributes;
//}

Game::Game()
{
	mpTimer = new Timer(); 
	//HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//mDefaultColors = GetConsoleTextAttribute(hConsole);
	theLobby = new Lobby();
	theGame = new GameLocalState();
	theState = theLobby;
}

Game::~Game()
{
	delete mpTimer;
	mpTimer = nullptr;
}

void Game::processLoop()
{
	//ApplicationState theState[1]; //both a pointer and object at the same time, we have the address to it by default
	//theState->running = 1;
//	ApplicationState *theState;

	theState = theLobby;
	double deltaTime = LOOP_TARGET_TIME;

	//do
	while(mRunning)
	{
		mpTimer->start();
		//updateInput(theState->key);
		//updateNetworking(theState);
		//updateState(theState);
		//render(theState);

		theState->updateInput();
		theState->updateNetworking();
		theState->updateState();
		theState->display();

		deltaTime = LOOP_TARGET_TIME - mpTimer->getElapsedTime();
		mpTimer->sleepUntilElapsed(deltaTime);
		mpTimer->stop();
		//std::cout << "Time for frame: " << deltaTime << std::endl;

	} //while (theState->running);
}

void Game::initGame()
{
	mpNetworkManager->initializeNetwork();
}

void Game::updateInput(KeyboardState * keyState)
{
	//-- get mouse, keyboard, controller state
	//-- track this for later

	//--example: in a chat room...
	GetKeyboardState(keyState->key);
}

void Game::SetTextRed()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
}

void Game::SetTextPurple()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
}

void Game::SetTextDefault()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), mDefaultColors);
}

//void Game::updateNetworking(ApplicationState * state)
//{
//	mpNetworkManager->updateServer();
//	mpNetworkManager->updateClient();
//}
//
//void Game::updateState(ApplicationState * state)
//{
//	//-- account for all previous updates here
//	//-- change application with respect to time and other updates
//
//	//-- e.g chat room
//	//-- update chat stream
//}
//
//void Game::render(const ApplicationState * state)
//{
//	//-- "clear" buffer
//	//-- display state to display medium
//
//	//e.g chat room
//	//-- display chat stream (still rendering)
//}
