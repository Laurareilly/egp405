
#ifndef  _GAMELOCALSTATE_H
#define  _GAMELOCALSTATE_H

#include "ApplicationState.h"

class GameLocalState : public ApplicationState
{
public:
	GameLocalState();
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
		data.doesUpdateNetworking = 1;
		data.doesUpdateState = 1;
		data.myUsername = passData->data.myUsername;
		data.portNumber = passData->data.portNumber;
		data.ipAddress = passData->data.ipAddress;
		data.headerMessage = "Welcome to UDPalooza!\nYou're live chatting now\nEnter #help for list of commands!";
	};

	virtual void goToNextState(ApplicationState *passData);
	virtual void clearCurrentMessage();
	virtual void processMessage();
	virtual void PushMessageIntoQueue();
	virtual void PushMessageIntoQueue(std::string newMessage);

	virtual void ClearScreen();
	virtual char NumberToSymbol(char numChar);

private:

};


#endif