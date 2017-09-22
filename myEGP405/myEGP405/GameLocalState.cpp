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
	data.headerMessage = "Welcome to UDPalooza!";
	for (int i = 0; i < 10; ++i) //our recent messages are blank (user hasnt input anything)
	{
		data.recentMessages[i] = '\n';
	}
	for (int i = 0; i < 20; ++i)
	{
		data.usernameList[i] = "";
	}
	data.currentChatMessage = "";
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

//THANKS TO COLIN BRADY FOR THIS LINK BELOW AND FOR SUGGESTING USING GETASYNCKEYSTATE INSTEAD OF SDL
//https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
//https://blog.molecular-matters.com/2011/09/05/properly-handling-keyboard-input/ this link showed me the MapVirtualKey and MAPVK_VK_TO_CHAR, which let me cast  to char
void GameLocalState::updateState()
{
	//Not using mouse inputs for this app, but there are lots of things to check still
	int index = 0;
	int shiftHeld = data.keyboardData[VK_SHIFT];

	//I'm kinda confident I understand why this works, but I might explain it wrong
	//I believe keyboardData has 256 switches, 1 for each possible input. but the value at 0x40 is undefined, and that's kinda dumb.
	//If, between the values for A-Z (or whatever for loop we're in), keyboard is "true"/"pressed down", we will push its character to the string we're typing
	//Then, we will update doesDisplay so we refresh the console and display the new data in the next function

	//message size limit is 95 characters, so I'll enforce that on everything except Enter and Backspace (I need the shift boolean above for backspace's shortcut)
	if (data.currentMessageIndex < 96)
	{
		//first, we will check for the letters A-Z (I'll add more once I prove this works)
		//link above says A Key is 0x41, z Key is 0x5A, so we'll parse through those values
		for (index = 0x41; index <= 0x5A; ++index)//using ++index cause dan likes it
		{
			if (data.keyboardData[index] && !data.prevKeyboardData[index]) //make sure it isn't just held down!
			{
				//we will append this character to our string!
				data.currentChatMessage += MapVirtualKey(index, MAPVK_VK_TO_CHAR);

				if (!shiftHeld)
					data.currentChatMessage[data.currentMessageIndex] += 32;

				//increment currentMessageIndex
				data.currentMessageIndex++;

				//we need to tell our display() to clear this frame and redraw
				data.doesDisplay = 1;
			}
		}

		//For menus, we should have 0-9. I don't think this is the same as for the numpad values. so dont try and use those it'll break my perfect inputmanager
		for (index = 0x30; index <= 0x39; ++index)//using ++index cause dan likes it
		{
			if (data.keyboardData[index] && !data.prevKeyboardData[index])//make sure it isn't just held down!
			{
				//we will append this character to our string!
				data.currentChatMessage += MapVirtualKey(index, MAPVK_VK_TO_CHAR);

				if (shiftHeld)
					data.currentChatMessage[data.currentMessageIndex] = NumberToSymbol(data.currentChatMessage[data.currentMessageIndex]);

				//increment currentMessageIndex
				data.currentMessageIndex++;

				//we need to tell our display() to clear this frame and redraw
				data.doesDisplay = 1;
			}
		}

		//Now, for some specifics. and I'm not giving users the \ key cause theyll hack my chat room or something.
		//Users for right now need Enter (to push their message through) and backspace (to delete that shit they didn't like);

		if (data.keyboardData[VK_SPACE] && !data.prevKeyboardData[VK_SPACE])//make sure it isn't just held down!
		{
			data.currentChatMessage += VK_SPACE;

			//increment currentMessageIndex
			data.currentMessageIndex++;

			//we need to tell our display() to clear this frame and redraw
			data.doesDisplay = 1;
		}
	}


	if (data.keyboardData[VK_RETURN] && !data.prevKeyboardData[VK_RETURN])//make sure it isn't just held down!
	{
		//this is a function, not a character to be inserted into the string
		processMessage();
	}

	//tried using \b but that caused so many issues. cant imagine how bad it would've been to send that through a packet, probably wouldnt be received well
	if (data.keyboardData[VK_BACK] && !data.prevKeyboardData[VK_BACK])//make sure it isn't just held down! this might be ok with backspace actually
	{
		if (data.currentMessageIndex == 0) //Y'A'INT deleting 0
			return;

		if (shiftHeld)
		{
			clearCurrentMessage();
		}
		else
		{
			//I tried using space but that doens't work either
			data.currentChatMessage.erase(data.currentMessageIndex - 1, 1);
			data.currentMessageIndex--; //this is pretty important huh
		}
		//we need to tell our display() to clear this frame and redraw
		data.doesDisplay = 1;
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

	ClearScreen(); //clear tj """""""""buffer""""""""" yoshi https://www.youtube.com/watch?v=kpk2tdsPh0A creedit to pannen for the meme

	cout << data.headerMessage << endl << endl << endl;

	for (int i = 9; i >= 0; i--)
	{
		if (data.recentMessages[i][0] == '@')
		{
			gpGame->SetTextPurple();
		}
		if (data.recentMessages[i][0] == '*')
		{
			gpGame->SetTextRed();
		}
		cout << data.recentMessages[i] << endl;
		gpGame->SetTextDefault();
	}

	//i put a '>' there cause it's like, CHAT HERE!!! haha it's good practice ok im very tired and hands r cold
	cout << ">" << data.currentChatMessage << "<\b";

	//verified with hard coding that server = 0 and client = 1
	int displayUserIndex = 0;
	if (mNetworkManager->mIsServer)
	{
		printf("\n\nUser List:");
		while (data.usernameList[displayUserIndex] != "")
		{
			if (displayUserIndex == 0) //mention we're the server!
			{
				printf("\n%s%: %s", /*(std::string)*/data.myUsername, "SERVER!");
			}
			else
			{
				printf("\n%s", data.usernameList[displayUserIndex]);
			}
			displayUserIndex++;
		}
	}
	
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
		if (data.currentChatMessage == "#help")
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
		}
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

