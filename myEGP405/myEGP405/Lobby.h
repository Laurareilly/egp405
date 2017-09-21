
#ifndef  _LOBBYSTATE_H
#define  _LOBBYSTATE_H

#include "ApplicationState.h"
#include <Windows.h>

class Lobby : public ApplicationState
{
public:
	Lobby();

	virtual char* getUsername() { return data.myUsername; }
	virtual void updateInput();
	virtual void AcceptedToServer() { data.enterServer = 1; }
	virtual void updateNetworking();

	virtual void updateState();

	virtual void display();
	virtual char** getUsernameList() { return data.usernameList; }
	virtual void SetSystemAddress(SystemAddress cAddress) {data.serverSystemAddress = cAddress;}
	virtual SystemAddress GetSystemAddress() { return data.serverSystemAddress; }
	virtual void SetClientID(int cID) { data.clientID = cID; }
	virtual void ReceiveMessage(char* cUser, char* cMessage, int cMsgType = 0) {};

	virtual void insertUsernameIntoList(char* cName, int cIndex) {};
	virtual int getNextOpenUsernameIndex() { return -1; };
	bool isServer;


	virtual void onArriveFromPrevious(ApplicationState *passData)
	{
		data.currentChatMessage = "";
		data.currentMessageIndex = 0;
		data.doesDisplay = 1;
		data.doesUpdateInput = 1;
		data.doesUpdateNetworking = 0;
		data.doesUpdateState = 1;
		strcpy(data.myUsername, passData->data.myUsername);
		data.portNumber = passData->data.portNumber;
		data.ipAddress = passData->data.ipAddress;
		data.headerMessage = "Welcome to UDPalooza!";
		mNetworkManager = passData->mNetworkManager;
		data.clientID = passData->data.clientID;
		data.serverSystemAddress = passData->data.serverSystemAddress;
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