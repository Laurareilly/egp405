#pragma once

class Timer;
class NetworkManager;

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
	void SetTextDefault();

	//struct ApplicationState
	//{
	//	int running;

	//	KeyboardState key[1];
	//};


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

