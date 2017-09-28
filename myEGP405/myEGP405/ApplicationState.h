//base class for state machine
/*
Lab 2
EGP 405-02
Laura Reilly -- 0972157
Robert Mitchell -- 0935286

We certify that this work is entirely our own.
The assessor of this project may reproduce this 
project and provide copies to other academic staff,
and/or communicate a copy of this project to a plagiarism
- checking service, which may retain a copy of the project on its database.”
*/
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
	virtual void AcceptedToServer() = 0;
	virtual void SetClientID(int cID) = 0;

	virtual void ForcePlayerToLobby() = 0;

	virtual void ReceiveSlotInput(int cSlot) = 0;
	virtual void StartGameAtPlayerTurn(int cTurn) = 0;

	virtual int GetCurrentTurn() = 0;

	//0 = general message, 1 = PM, 2 = console message(red)
	virtual void ReceiveMessage(char* cUser, char* cMessage, int cMsgType = 0) = 0;

	virtual void SetPeerAddress(SystemAddress cAddress) = 0;
	virtual SystemAddress GetPeerAddress() = 0;
	virtual void PlayerJoined() = 0;

	virtual int getNextOpenUsernameIndex() = 0;
	virtual void insertUsernameIntoList(char* cName, int cIndex) = 0;

	virtual int getIsLocal() = 0;

	virtual char** getUsernameList() = 0;
	virtual char* getUsername() = 0;

	bool isServer;

	ApplicationState() { mNetworkManager = new NetworkManager(); }
	~ApplicationState() { delete mNetworkManager; mNetworkManager = NULL; }

	SystemAddress allSystemAddresses[20]; //why isnt this working

protected:
	ApplicationState *next, *previous;

	struct ApplicationStateData
	{
		SystemAddress peerSystemAddress; //this is the system address of our peer, we send data to them exclusively, this is overwritten when new opponent
		unsigned int isLocal = 1; // 0 means networked
		unsigned int clientID = 1;
		char* usernameList[20]; //max clients is TWENTY
		char doesUpdateInput;
		char doesUpdateNetworking;
		char doesUpdateState;
		char doesDisplay;
		char enterServer = 0;
		int currentMessageIndex = 0; //I really should know where I'm typing. I didn't have this for a while, but things got super messed up when trying to implement backspace, not sure if i need it anymore though
		int keyboardData[256]; //will be set to get async key thing
		int prevKeyboardData[256]; //this will be compared to keyboardData to force users to press each key as they want to type it (you can't hold down A and have it type AAAAAAAAA, you have to type each idnividual A)
		std::string currentChatMessage;
		std::string headerMessage;
		std::string recentMessages[10];
		char myUsername[31];
		int portNumber = 5546; //hard setting this beautiful port numbre that works consistently as far as i can tell
		char* ipAddress = "127.0.0.1";
		//previous char buffer
	} data;

	int refreshDelayMax = 3, refreshDelay = 0;

	virtual void goToNextState(ApplicationState *passData) = 0;
	virtual void onArriveFromPrevious(ApplicationState *passData) = 0;

	NetworkManager *mNetworkManager;

};



#endif // ! _APPLICATIONSTATE_H
