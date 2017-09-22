/*
Project 1
EGP 405-02
Laura Reilly -- 0972157
Robert Mitchell -- oh no

We certify that this work is entirely our own.
The assessor of this project may reproduce this
project and provide copies to other academic staff,
and/or communicate a copy of this project to a plagiarism
- checking service, which may retain a copy of the project on its database.”
*/


#ifndef  _GAMELOCALSTATE_H
#define  _GAMELOCALSTATE_H

#include "ApplicationState.h"

class GameLocalState : public ApplicationState
{
public:
	GameLocalState();
	virtual char* getUsername() { return data.myUsername; }
	virtual void updateInput();
	virtual void updateNetworking();
	virtual void updateState();
	virtual void display();
	virtual void AcceptedToServer() { data.enterServer = 1; }
	virtual char** getUsernameList() { return data.usernameList; }
	virtual int getNextOpenUsernameIndex();
	virtual void insertUsernameIntoList(char cName[31], int cIndex);
	virtual void SetSystemAddress(SystemAddress cAddress) { data.serverSystemAddress = cAddress; }
	virtual SystemAddress GetSystemAddress() { return data.serverSystemAddress; }
	virtual void SetClientID(int cID) 
	{
		data.clientID = (unsigned int)cID; 
		printf("%f", data.clientID);
	}
	virtual void ReceiveMessage(char cUser[31], char cMessage[96], int cMsgType = 0);
	virtual void onArriveFromPrevious(ApplicationState *passData) 
	{
		data.currentChatMessage = "";
		data.currentMessageIndex = 0;
		data.doesDisplay = 1;
		data.doesUpdateInput = 1;
		data.doesUpdateNetworking = 1;
		data.doesUpdateState = 1;
		strcpy(data.myUsername, passData->data.myUsername);
		data.portNumber = passData->data.portNumber;
		data.ipAddress = passData->data.ipAddress;
		data.headerMessage = "Welcome to UDPalooza!\nYou're live chatting now\nEnter #help for list of commands or #quit to leave!";
		mNetworkManager = passData->mNetworkManager;
		data.clientID = passData->data.clientID;
		data.serverSystemAddress = passData->data.serverSystemAddress;
		if (mNetworkManager->mIsServer)
		{
			data.clientID = 0;
			getUsernameList()[0] = data.myUsername;
		}
	};
	bool isServer;
	virtual void goToNextState(ApplicationState *passData);
	virtual void clearCurrentMessage();
	virtual void processMessage();
	virtual void PushMessageIntoQueue();
	virtual void PushMessageIntoQueue(std::string newMessage);

	virtual void ClearScreen();
	virtual char NumberToSymbol(char numChar);

	char newUsername[20][31]; //my version of allocating space for 20 usernames
	//i might be able to use this INSTEAD of username list but I'm scared to change much
	//this was just a char [31], but I realized having additional clients would overwrite the data stored here
	//as always, I learned a lot about C from this project even though I'm POSITIVE this is improper 

private:

};


#endif