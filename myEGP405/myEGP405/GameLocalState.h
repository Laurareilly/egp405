
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
		data.clientID = cID; 
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
		data.headerMessage = "Welcome to UDPalooza!\nYou're live chatting now\nEnter #help for list of commands!";
		mNetworkManager = passData->mNetworkManager;
		data.clientID = passData->data.clientID;
		data.serverSystemAddress = passData->data.serverSystemAddress;
		if (mNetworkManager->mIsServer)
		{
			data.clientID = 0;
			getUsernameList()[0] = data.myUsername;
			data.headerMessage = getUsernameList()[0];
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

	char newUsername[31];

private:

};


#endif