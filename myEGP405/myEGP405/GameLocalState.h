/*
Project 1
EGP 405-02
Laura Reilly -- 0972157
Robert Mitchell -- 0935286

We certify that this work is entirely our own.
The assessor of this project may reproduce this
project and provide copies to other academic staff,
and/or communicate a copy of this project to a plagiarism
- checking service, which may retain a copy of the project on its database.”
*/


#ifndef  _GAMELOCALSTATE_HpeerSystemAddress
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
	virtual void SetPeerAddress(SystemAddress cAddress) { data.peerSystemAddress = cAddress; }
	virtual void PlayerJoined() { waitingForPlayer = false; }
	virtual SystemAddress GetPeerAddress() { return data.peerSystemAddress; }

	virtual int GetCurrentTurn() { return playerTurn; }

	virtual void StartGameAtPlayerTurn(int cTurn) { waitingForPlayer = false; playerTurn = startPlayerTurn = cTurn; }

	virtual void ForcePlayerToLobby();

	virtual int getIsLocal() { return data.isLocal; }

	virtual void SetClientID(int cID) 
	{
		data.clientID = (unsigned int)cID; 
		printf("%f", data.clientID);
	}
	virtual void ReceiveMessage(char cUser[31], char cMessage[96], int cMsgType = 0);
	virtual void ReceiveSlotInput(int cSlot);

	virtual void onArriveFromPrevious(ApplicationState *passData) 
	{
		waitingForPlayer = true;
		data.isLocal = passData->data.isLocal;
		data.currentChatMessage = "";
		data.currentMessageIndex = 0;
		data.doesDisplay = 1;
		data.doesUpdateInput = 1;
		data.doesUpdateNetworking = 1;
		data.doesUpdateState = 0;
		strcpy(data.myUsername, passData->data.myUsername);
		data.portNumber = passData->data.portNumber;
		data.ipAddress = passData->data.ipAddress;
		if (data.isLocal)
			data.headerMessage = "Youre in-game!\nWASD for P1 | IJKL for P2\nPress Enter to Confirm Move\nPress ESC to return to lobby\nPress SHIFT+ESC to quit application";
		else
			data.headerMessage = "Youre in-game!\nWASD to choose slot on game board\nPress Enter to Confirm Move\nPress ESC to return to lobby\nPress SHIFT+ESC to quit application";
		mNetworkManager = passData->mNetworkManager;
		data.clientID = passData->data.clientID;
		data.peerSystemAddress = passData->data.peerSystemAddress;
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
	virtual void SendMoveToOpponent(int moveSlot);

	virtual void ClearScreen();
	virtual char NumberToSymbol(char numChar);

	char newUsername[20][31]; //my version of allocating space for 20 usernames
	//i might be able to use this INSTEAD of username list but I'm scared to change much
	//this was just a char [31], but I realized having additional clients would overwrite the data stored here
	//as always, I learned a lot about C from this project even though I'm POSITIVE this is improper 

private:
	bool p1Up, p1Down, p1Left, p1Right, enterPressed, escPressed, shiftHeld, p2Up, p2Down, p2Left, p2Right; //p2 will be IJKL or arrow keys probbly
	void updateStateLocalGame();
	void updateStateNetworkedGame();
	void resetGame(bool isNetworked = false);

	bool validateMove();
	int setMove();
	void checkForWin(unsigned int playerNum);

	bool waitingForPlayer = true;

	int slotIndex = 0; //read from left to right, top to bottom 0 - 8
	unsigned int playerTurn = 0; //0 is player 1 ofc. In a networked game, compare to clientID for validating input
	unsigned int startPlayerTurn = 0;
	char slotArray[9];
	int slotData[9];
	unsigned int moveCounter;
	bool gameOver;
};


#endif