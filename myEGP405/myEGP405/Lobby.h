/*
Project 1
EGP 405-02
Laura Reilly -- 0972157
Robert Mitchell -- oh no

We certify that this work is entirely our own.
The assessor of this project may reproduce this
project and provide copies to other academic staff,
and/or communicate a copy of this project to a plagiarism
- checking service, which may retain a copy of the project on its database.�
*/


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

	virtual int getIsLocal() { return data.isLocal; }

	virtual void insertUsernameIntoList(char* cName, int cIndex) {};
	virtual int getNextOpenUsernameIndex() { return -1; };
	bool isServer;


	virtual void onArriveFromPrevious(ApplicationState *passData)
	{
		data.isLocal = passData->data.isLocal;
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
		FIRST_SCREEN, //local, network, quit
		CHOSE_NETWORKED_GAME, //join or host
		//MAKING_SERVER, //host automatically does this (it gets skipped)
		//JOINING_SERVER, //skip thise
		JOINING_SERVER_IP, //enter an IP, then it enters Estalbishing connectiohn, and when it's in here it tries to connect for 30 frames and then jumps back to joining srver IP
		ESTABLISHING_CONNECTION, //no input is considered here, just waits for 30 frames OR connects within tat time
	}mCurrentOption = FIRST_SCREEN;

	bool wantsToBeServer = false;
	bool successfullyConnectedToServer = false;
	int waitFrames = 30;
};


#endif