#pragma once

#include <Windows.h>
#include "RakNet\RakPeerInterface.h"

class Game
{
public:

	Game();
	~Game();

	void beginLoop();
	void processLoop();
	bool endLoop();

	struct KeyboardState
	{
		unsigned char key[256];
	};


	struct ApplicationState
	{
		int running;

		KeyboardState key[1];
	};


private:

	void updateInput(KeyboardState* keyState);
	void updateNetworking(ApplicationState* state);
	void updateState(ApplicationState* state);
	void render(const ApplicationState* state);
	void sleepUntilNextFrame();


	const float LOOP_TARGET_TIME = 33.0f; //30 fps
	const float TARGET_ELAPSED_TIME = LOOP_TARGET_TIME / 1000.0f;
	double mElapsedTime;
	//auto mStartTime;
	bool mShouldExit;
};

