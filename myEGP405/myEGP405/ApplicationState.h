//base class for state machine

#ifndef  _APPLICATIONSTATE_H
#define  _APPLICATIONSTATE_H

#include <string>

#include "NetworkManager.h"

class ApplicationState abstract //this type of state can never be used
{
public:
	friend class Lobby;
	friend class GameLocalState;
	virtual void updateInput() = 0;
	virtual void updateNetworking() = 0;
	virtual void updateState() = 0;
	virtual void display() = 0;
	virtual void clearCurrentMessage() = 0;
	virtual void processMessage() = 0;
	virtual void PushMessageIntoQueue() = 0;
	virtual void ClearScreen() = 0;
	virtual char NumberToSymbol(char numChar) = 0;

	ApplicationState() { mNetworkManager = new NetworkManager(); }
	~ApplicationState() { delete mNetworkManager; mNetworkManager = NULL; }

protected:
	ApplicationState *next, *previous;

	struct ApplicationStateData
	{
		char doesUpdateInput;
		char doesUpdateNetworking;
		char doesUpdateState;
		char doesDisplay;
		int currentMessageIndex = 0; //I really should know where I'm typing. I didn't have this for a while, but things got super messed up when trying to implement backspace, not sure if i need it anymore though
		int keyboardData[256]; //will be set to get async key thing
		int prevKeyboardData[256]; //this will be compared to keyboardData to force users to press each key as they want to type it (you can't hold down A and have it type AAAAAAAAA, you have to type each idnividual A)
		std::string currentChatMessage;
		std::string headerMessage;
		std::string recentMessages[10];
		std::string myUsername = "";
		int portNumber = 5546;
		char* ipAddress = "127.0.0.1";
		//previous char buffer
	} data;

	int refreshDelayMax = 3, refreshDelay = 0;

	virtual void goToNextState(ApplicationState *passData) = 0;
	virtual void onArriveFromPrevious(ApplicationState *passData) = 0;

	NetworkManager *mNetworkManager;
};



#endif // ! _APPLICATIONSTATE_H
