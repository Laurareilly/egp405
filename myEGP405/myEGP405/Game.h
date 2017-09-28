/*
Project 1
EGP 405-02
Laura Reilly -- 0972157
Robert Mitchell -- 0935286

We certify that this work is entirely our own.
The assessor of this project may reproduce this
project and provide copies to other academic staff,
and/or communicate a copy of this project to a plagiarism
- checking service, which may retain a copy of the project on its database.”
*/

#pragma once

class Timer;
class NetworkManager;
class Lobby;
class GameLocalState;
class ApplicationState;

class Game
{
public:

	Game();
	~Game();

	void processLoop();

	struct KeyboardState
	{
		unsigned char key[256];
	};

	void requestExit() { mRunning = false; }

	void SetTextRed();
	void SetTextPurple();
	void SetTextGreen();
	void SetTextBlue();
	void SetTextDefault();

	//struct ApplicationState
	//{
	//	int running;

	//	KeyboardState key[1];
	//};

	Lobby *theLobby;
	GameLocalState *theGame; 
	ApplicationState *theState;

private:

	void initGame();
	void updateInput(KeyboardState* keyState);
	//void updateNetworking(ApplicationState* state);
	//void updateState(ApplicationState* state);
	//void render(const ApplicationState* state);


	const float LOOP_TARGET_TIME = 33.0f; //30 fps
	const float TARGET_ELAPSED_TIME = LOOP_TARGET_TIME / 1000.0f;
	bool mRunning = true;

	Timer *mpTimer;
	NetworkManager *mpNetworkManager;
	int mDefaultColors;
};

