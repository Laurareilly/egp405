
#ifndef  _LOBBYSTATE_H
#define  _LOBBYSTATE_H

#include "ApplicationState.h"
#include <Windows.h>

class Lobby : public ApplicationState
{
public:
	Lobby();

	virtual void updateInput();

	virtual void updateNetworking();

	virtual void updateState();

	virtual void display();

	virtual void onArriveFromPrevious(ApplicationState *passData)
	{
		data.currentChatMessage = "";
		data.currentMessageIndex = 0;
		data.doesDisplay = 1;
		data.doesUpdateInput = 1;
		data.doesUpdateNetworking = 0;
		data.doesUpdateState = 1;
		data.myUsername = passData->data.myUsername;
		data.portNumber = passData->data.portNumber;
		data.ipAddress = passData->data.ipAddress;
		data.headerMessage = "Welcome to UDPalooza!";
	}

	virtual void goToNextState(ApplicationState *passData);

	virtual void clearCurrentMessage();
	virtual void processMessage();
	virtual void PushMessageIntoQueue();
	virtual void PushMessageIntoQueue(std::string newMessage);

	virtual void ClearScreen();
	virtual char NumberToSymbol(char numChar);

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
	bool successfullyConnectedToServer = false;
	int waitFrames = 30;
};


#endif