
#ifndef  _LOBBYSTATE_H
#define  _LOBBYSTATE_H

#include "ApplicationState.h"
#include <Windows.h>

class Lobby : public ApplicationState
{
public:
	Lobby();

	virtual void updateInput();

	virtual void updateNetworking()
	{
		//for lobby, this would just init server or attempt a connection i think
	}

	virtual void updateState();

	virtual void display();

	virtual void onArriveFromPrevious(ApplicationState *passData)
	{

	}

	virtual void goToNextState(ApplicationState *passData)
	{
		//next->onArriveFromPrevious(passData);
	}

	virtual void clearCurrentMessage();
	virtual void processMessage();
	virtual void PushMessageIntoQueue();
	virtual void PushMessageIntoQueue(std::string newMessage);

private:
	std::string lobbyOptionText;
	enum CurrentChoice
	{
		FIRST_SCREEN,
		MAKING_SERVER,
		JOINING_SERVER,
		JOINING_SERVER_IP,
		ESTABLISHING_CONNECTION,
	}mCurrentOption = FIRST_SCREEN;

	bool wantsToBeServer = false;
};


#endif