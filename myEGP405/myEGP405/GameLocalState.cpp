#include "GameLocalState.h"
#include "ApplicationState.h"
#include "Lobby.h"
#include "commonFile.h"
#include "Game.h"
#include <iostream>
#include <string>

//Game *gpGame;

GameLocalState::GameLocalState()
{
	data.headerMessage = "Welcome to Tic Tac Toe!";
	for (int i = 0; i < 10; ++i) //our recent messages are blank (user hasnt input anything)
	{
		data.recentMessages[i] = '\n';
	}
	for (int i = 0; i < 20; ++i)
	{
		data.usernameList[i] = "";
	}
	data.currentChatMessage = "";
	int i = 0;
	for (i = 0; i < 6; ++i)
	{
		slotArray[i] = '_';
		slotData[i] = -1;
	}
	for (i = 6; i < 9; ++i)
	{
		slotArray[i] = ' ';
		slotData[i] = -1;
	}
	moveCounter = 0;
	gameOver = false;
}

void GameLocalState::updateInput()
{
	if ((GetConsoleWindow() == GetForegroundWindow())) //the secret weapon 
	{
		for (int i = 0; i < 256; ++i)
		{
			data.keyboardData[i] = GetAsyncKeyState(i);
		}
	}
	else
	{
		for (int i = 0; i < 256; ++i)
		{
			data.keyboardData[i] = 0;
		}
	}
}


//couldnt quickly think of a good alg for this so we hardset these 10 char conversions
char GameLocalState::NumberToSymbol(char numChar)
{
	switch (numChar)
	{
	case '1':
	{
		return '!';
	}
	break;
	case '2':
	{
		return '@';
	}
	break;
	case '3':
	{
		return '#';
	}
	break;
	case '4':
	{
		return '$';
	}
	break;
	case '5':
	{
		return '%';
	}
	break;
	case '6':
	{
		return '^';
	}
	break;
	case '7':
	{
		return '&';
	}
	break;
	case '8':
	{
		return '*';
	}
	break;
	case '9':
	{
		return '(';
	}
	break;
	case '0':
	{
		return ')';
	}
	break;

	default:
	{
		return numChar;
	}
	break;
	}

}

void GameLocalState::clearCurrentMessage()
{
	data.currentChatMessage.erase(0, data.currentMessageIndex);
	data.currentMessageIndex = 0;
}

void GameLocalState::updateStateLocalGame()
{
	//Local game specific input booleans
	p2Up = (data.keyboardData[0x49] && !data.prevKeyboardData[0x49]); //I
	p2Down = (data.keyboardData[0x4B] && !data.prevKeyboardData[0x4B]); //K
	p2Left = (data.keyboardData[0x4A] && !data.prevKeyboardData[0x4A]); //J
	p2Right = (data.keyboardData[0x4C] && !data.prevKeyboardData[0x4C]); //L

	if (shiftHeld && escPressed)
	{
		gpGame->requestExit();
	}
	else if (escPressed)
	{
		//yeh
		goToNextState(this);
		resetGame();
	}

	if (!gameOver)
	{
		if (playerTurn == 0)
		{
			if (p1Up)
			{
				slotIndex -= 3;
			}
			else if (p1Down)
			{
				slotIndex += 3;
			}
			if (slotIndex < 0)
				slotIndex += 9;
			if (slotIndex > 8)
				slotIndex -= 9;

			if (p1Right)
			{
				slotIndex++;
				if (slotIndex % 3 == 0)
					slotIndex -= 3;
			}
			else if (p1Left)
			{
				slotIndex--;
				if ((slotIndex + 4) % 3 == 0)
					slotIndex += 3;
			}
		}
		else if (playerTurn == 1)
		{
			if (p2Up)
			{
				slotIndex -= 3;
			}
			else if (p2Down)
			{
				slotIndex += 3;
			}
			if (slotIndex < 0)
				slotIndex += 9;
			if (slotIndex > 8)
				slotIndex -= 9;

			if (p2Right)
			{
				slotIndex++;
				if (slotIndex % 3 == 0)
					slotIndex -= 3;
			}
			else if (p2Left)
			{
				slotIndex--;
				if ((slotIndex + 4) % 3 == 0)
					slotIndex += 3;
			}
		}

		if (enterPressed)//make sure it isn't just held down!
		{
			if (validateMove())
			{
				setMove();
			}
			else
			{
				data.doesDisplay = 1;
				data.recentMessages[0] = "Invalid Move! Please try a different slot!";
			}
		}
	}
	else
	{
		//is game over
		//maybe we might want to move this somewhere else, but its really only checking 3 nums at end game so

		//1. Restart game
		if (data.keyboardData[0x31] && !data.prevKeyboardData[0x31]) //make sure it isn't just held down!
		{
			resetGame();
		}

		//2. Back to Lobby
		if (data.keyboardData[0x32] && !data.prevKeyboardData[0x32]) //make sure it isn't just held down!
		{
			resetGame();
			goToNextState(this);
		}
		//3. Exit
		if (data.keyboardData[0x33] && !data.prevKeyboardData[0x33]) //make sure it isn't just held down!
		{
			gpGame->requestExit();
		}
	}
	
}

void GameLocalState::updateStateNetworkedGame()
{
	//Additional networking inputs go here (PRESS 1-9 FOR FUN COLORS?)
	if (shiftHeld && escPressed)
	{
		gpGame->requestExit();
	}
	else if (escPressed)
	{
		goToNextState(this);
		resetGame();
	}

	if (playerTurn != data.clientID)
	{
		return;
	}

	if (p1Up)
	{
		slotIndex -= 3;
	}
	else if (p1Down)
	{
		slotIndex += 3;
	}
	if (slotIndex < 0)
		slotIndex += 9;
	if (slotIndex > 8)
		slotIndex -= 9;

	if (p1Right)
	{
		slotIndex++;
		if (slotIndex % 3 == 0)
			slotIndex -= 3;
	}
	else if (p1Left)
	{
		slotIndex--;
		if ((slotIndex + 4) % 3 == 0)
			slotIndex += 3;
	}


	if (enterPressed)//make sure it isn't just held down!
	{
		if (validateMove())
		{
			SendMoveToOpponent(setMove());			
		}
		else
		{
			data.doesDisplay = 1;
			data.recentMessages[0] = "Invalid Move! Please try a different slot!";
		}
	}
}

//using it more as a "reset" game i guess
void GameLocalState::resetGame()
{
	gameOver = false;
	data.headerMessage = "Youre in-game!\nWASD for P1 | IJKL for P2\nPress Enter to Confirm Move\nPress ESC to return to lobby\nPress SHIFT+ESC to quit application";
	for (int i = 0; i < 10; ++i) //our recent messages are blank (user hasnt input anything)
	{
		data.recentMessages[i] = '\n';
	}
	for (int i = 0; i < 20; ++i)
	{
		data.usernameList[i] = "";
	}
	data.currentChatMessage = "";
	int i = 0;
	for (i = 0; i < 6; ++i)
	{
		slotArray[i] = '_';
		slotData[i] = -1;
	}
	for (i = 6; i < 9; ++i)
	{
		slotArray[i] = ' ';
		slotData[i] = -1;
	}
	playerTurn = 1 - startPlayerTurn;
	startPlayerTurn = playerTurn;
	//playerTurn = 0;//<-not this
	moveCounter = 0;
	data.doesDisplay = 1;
}

void GameLocalState::updateState()
{
	//All input checks required for both local and networked games
	if (data.doesUpdateState == 0)
	{
		data.doesUpdateState = 1;
		return;
	}

	p1Up = (data.keyboardData[0x57] && !data.prevKeyboardData[0x57]); //W
	p1Down = (data.keyboardData[0x53] && !data.prevKeyboardData[0x53]); //S
	p1Left = (data.keyboardData[0x41] && !data.prevKeyboardData[0x41]); //A
	p1Right = (data.keyboardData[0x44] && !data.prevKeyboardData[0x44]); //D

	enterPressed = (data.keyboardData[VK_RETURN] && !data.prevKeyboardData[VK_RETURN]); //Enter
	escPressed = (data.keyboardData[VK_ESCAPE] && !data.prevKeyboardData[VK_ESCAPE]); //Escape
	shiftHeld = data.keyboardData[VK_SHIFT]; //Shift

	if (data.isLocal)
	{
		updateStateLocalGame();
	}
	else
	{
		updateStateNetworkedGame();
	}

	if (p1Up || p1Down || p1Left || p1Right || enterPressed || escPressed || p2Up || p2Down || p2Left || p2Right)
	{
		data.doesDisplay = 1;

		//update slotArray with correct strings
		for (int i = 0; i < 9; i++)
		{
			if (slotData[i] == -1)
			{
				slotArray[i] = i > 5 ? ' ' : '_';
			}
			if (slotData[i] == 0)
				slotArray[i] = 'X';
			if (slotData[i] == 1)
				slotArray[i] = 'O';
		}

		if (data.isLocal)
		{
			if (playerTurn == 0)
				slotArray[slotIndex] = 'x';
			else
				slotArray[slotIndex] = 'o';
		}
		else
		{
			//if it's our turn, overwrite at slot index with our specific character (also it has to be GREEN?)
			if (playerTurn == data.clientID)
			{
				if (playerTurn == 0)
					slotArray[slotIndex] = 'x';
				else
					slotArray[slotIndex] = 'o';
			}
		}

		//if it's networked, and not our turn, we don't care about slotIndex
		//otherwise, we do take this into account
	}

	for (int i = 0; i < 256; i++) //might not call this if !doesDisplay, but that might be TOO meta (this is safer and it's just 256 ints so w/e
	{
		//so long space cowboy
		data.prevKeyboardData[i] = data.keyboardData[i];
	}

}

std::string HelpMessage()
{
	return "SHORTCUTS:\n @ followed by a username whispers to that user.\nStart a message with * to make it RED!";

}

//I thought this was gonna be more elaborate. I'd have to make it so if this was a game, not a chat room. Or if it was a more active chatroom even
void GameLocalState::ClearScreen()
{
	system("CLS");
}

void ChangeColorBasedOnPassedChar(char cChar)
{
	if (cChar == 'X')
		gpGame->SetTextRed();
	if (cChar == 'O')
		gpGame->SetTextBlue();
	if (cChar == 'x' || cChar == 'o')
		gpGame->SetTextGreen();
}

void displayGameBoard(char cArray[9])
{
	using namespace std;
	cout << "_";
	ChangeColorBasedOnPassedChar(cArray[0]);
	cout << cArray[0];
	gpGame->SetTextDefault();
	cout << "_|_";
	ChangeColorBasedOnPassedChar(cArray[1]);
	cout << cArray[1];
	gpGame->SetTextDefault();
	cout << "_|_";
	ChangeColorBasedOnPassedChar(cArray[2]);
	cout << cArray[2];
	gpGame->SetTextDefault();
	cout << "_" << endl;
	cout << "_";
	ChangeColorBasedOnPassedChar(cArray[3]);
	cout << cArray[3];
	gpGame->SetTextDefault();
	cout << "_|_";
	ChangeColorBasedOnPassedChar(cArray[4]);
	cout << cArray[4];
	gpGame->SetTextDefault();
	cout << "_|_";
	ChangeColorBasedOnPassedChar(cArray[5]);
	cout << cArray[5];
	gpGame->SetTextDefault();
	cout << "_" << endl;
	cout << " ";
	ChangeColorBasedOnPassedChar(cArray[6]);
	cout << cArray[6];
	gpGame->SetTextDefault();
	cout << " | ";
	ChangeColorBasedOnPassedChar(cArray[7]);
	cout << cArray[7];
	gpGame->SetTextDefault();
	cout << " | ";
	ChangeColorBasedOnPassedChar(cArray[8]);
	cout << cArray[8];
	gpGame->SetTextDefault();
	cout << " " << endl;
}

void GameLocalState::display()
{
	using namespace std;
	refreshDelay--;
	if (refreshDelay > 0)
	{
		return;
	}

	refreshDelay = refreshDelayMax;

	if (!data.doesDisplay)
	{
		return;
	}
	data.doesDisplay = 0;

	ClearScreen(); //clear screen

	cout << data.headerMessage << endl << endl << endl;

	displayGameBoard(slotArray);

	string controlSchemeTemp = "";
	if (playerTurn == 0 || !data.isLocal)
	{
		controlSchemeTemp = "Use WASD to move cursor!";
	}
	else if (playerTurn == 1)
	{
		controlSchemeTemp = "Use IJKL to move cursor!";
	}

	gpGame->SetTextPurple();
	cout << endl << "Current Player: " << playerTurn << endl << controlSchemeTemp << endl;

	gpGame->SetTextRed();
	cout << endl << data.recentMessages[0] << endl << endl;
	gpGame->SetTextDefault();
	cout.flush();
}

int GameLocalState::getNextOpenUsernameIndex()
{
	for (int i = 0; i < 20; ++i)
	{
		if (data.usernameList[i] == "")
			return i;
	}
	return -1;
}

void GameLocalState::insertUsernameIntoList(char cName[31], int cIndex)
{	
	strcpy(newUsername[cIndex], cName);
	data.usernameList[cIndex] = newUsername[cIndex];
	strcpy(data.usernameList[cIndex], cName);
	data.doesDisplay = 1;
}

//proof of concept, move the message up with "User: " at the beginning, then allow users to type a new one
void GameLocalState::PushMessageIntoQueue()
{
	for (int i = 9; i > 0; i--)
	{
		data.recentMessages[i] = data.recentMessages[i - 1];
	}

	data.recentMessages[0] = "User: " + data.currentChatMessage + "\n";
	clearCurrentMessage();
	data.doesDisplay = 1;
}

void GameLocalState::PushMessageIntoQueue(std::string newMessage)
{
	for (int i = 9; i > 0; i--)
	{
		data.recentMessages[i] = data.recentMessages[i - 1];
	}

	data.recentMessages[0] = newMessage + '\n';
	clearCurrentMessage();
	data.doesDisplay = 1;
}

void GameLocalState::processMessage()
{
	if (data.currentMessageIndex > 0)
	{
		if (data.currentChatMessage == "1")
		{
			//play again
		}
		if (data.currentChatMessage == "2")
		{
			//go back to lobby
		}
		if (data.currentChatMessage == "3")
		{
			//exit
			gpGame->requestExit();
		}
		/*if (data.currentChatMessage == "#help")
		{
			PushMessageIntoQueue(HelpMessage());
		}
		if (data.currentChatMessage == "#quit")
		{
			gpGame->requestExit();
			mNetworkManager->SendNetworkedMessage(" has disconnected. ", data.clientID);
		}
		else
		{
			char* myMessage = new char[data.currentChatMessage.length() + 1];
			strcpy(myMessage, data.currentChatMessage.c_str());
			if (mNetworkManager->mIsServer)
			{
				ReceiveMessage(data.myUsername, myMessage);
			}
			mNetworkManager->SendNetworkedMessage(myMessage, data.clientID);
			clearCurrentMessage();
		}*/
	}
}


void GameLocalState::goToNextState(ApplicationState *passData)
{
	gpGame->theState = gpGame->theLobby;
	next = gpGame->theState;
	next->onArriveFromPrevious(passData);
}

void GameLocalState::updateNetworking()
{
	if (data.isLocal)
		return;
	mNetworkManager->updateServer();
}



void GameLocalState::ReceiveMessage(char cUser[31], char cMessage[96], int cMsgType)
{
	std::string userString = cUser;
	std::string messageString = cMessage;
	std::string fullString = "";
	if (cMessage[0] == '*')//the LEAST elegant way to do this delighter
	{
		if (messageString.length() > 1) //if ALL they typed is *, we make it a space so they jsut have a blank message with a red name (still WAYYYY cool)S
			messageString.erase(0, 1);
		else		
			messageString = " ";
		
		fullString = '*' + userString + ": " + messageString;
	}
	else
		fullString = userString + ": " + messageString;

	data.doesDisplay = 1;
	PushMessageIntoQueue(fullString);
}

bool GameLocalState::validateMove()
{
	return (slotData[slotIndex] == -1);
}

int GameLocalState::setMove()
{
	int tempSlot = slotIndex;
	data.recentMessages[0] = "";
	slotData[slotIndex] = playerTurn;
	moveCounter++;
	checkForWin(playerTurn);
	playerTurn = 1 - playerTurn;
	slotIndex = 0;
	data.doesDisplay = 1;
	return tempSlot;
}

//We've already validated our move
//We've already set it on our screen
//We've already set the turn to be the other player's on our instance
//Now we need to send our completely valid move to them so they can also call setMove()
//This will also make it so it becomes their turn and they can send a move
//Alternatively, if the game ends, it will automatically detect that on both PEERS
void GameLocalState::SendMoveToOpponent(int moveSlot)
{
	mNetworkManager->SendNetworkedMove(moveSlot);
}

void GameLocalState::checkForWin(unsigned int playerNum)
{
	data.doesDisplay = 1;
	//ok here we go
	if (slotData[0] == playerNum && slotData[1] == playerNum && slotData[2] == playerNum || //checking top row
		slotData[3] == playerNum && slotData[4] == playerNum && slotData[5] == playerNum || //checking middle row
		slotData[6] == playerNum && slotData[7] == playerNum && slotData[8] == playerNum || //checking bottom row
		slotData[0] == playerNum && slotData[3] == playerNum && slotData[6] == playerNum || //checking first column
		slotData[1] == playerNum && slotData[4] == playerNum && slotData[7] == playerNum || //checking second column
		slotData[2] == playerNum && slotData[5] == playerNum && slotData[8] == playerNum || //checking third column
		slotData[0] == playerNum && slotData[4] == playerNum && slotData[8] == playerNum || //checking diagonal
		slotData[2] == playerNum && slotData[4] == playerNum && slotData[6] == playerNum ) //checking diagonal
	{
		if (playerNum == 0)
		{
			gameOver = true;
			PushMessageIntoQueue("Player 1 wins! Play again?\n1.Play Again\n2.Back to Lobby\n3.Quit Game");
			return;
		}
		else
		{
			gameOver = true;
			PushMessageIntoQueue("Player 2 wins! Play again?\n1.Play Again\n2.Back to Lobby\n3.Quit Game");
			return;
		}
	}
	if (moveCounter >= 9)
	{
		gameOver = true;
		PushMessageIntoQueue("IT WAS A TIE! Play again?\n1.Play Again\n2.Back to Lobby\n3.Quit Game");
		return;
	}
}

void GameLocalState::ReceiveSlotInput(int cSlot)
{
	//it's already validated on the other PEER's end and we are just taking their totally valid move and display it (updating our data)
	slotIndex = cSlot;
	setMove();
}