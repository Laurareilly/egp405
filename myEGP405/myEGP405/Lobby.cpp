#include "Lobby.h"
#include "commonFile.h"
#include "Game.h"
#include <iostream>
#include <string>
#include "GameLocalState.h"
#include "ApplicationState.h"
#include "GameMessage.h"
#include "Raknet/MessageIdentifiers.h"

//Game *gpGame;

Lobby::Lobby()
{
	data.headerMessage = "Welcome to UDPalooza!";
	for (int i = 0; i < 10; ++i) //our recent messages are blank (user hasnt input anything)
	{
		data.recentMessages[i] = '\n';
	}
	data.currentChatMessage = "";
	lobbyOptionText = "Please enter your username (no spaces): ";
	data.doesUpdateNetworking = 0;
}

void Lobby::updateInput()
{
	if ((GetConsoleWindow() == GetForegroundWindow()))
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


char Lobby::NumberToSymbol(char numChar)
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

void Lobby::clearCurrentMessage()
{
	data.currentChatMessage.erase(0, data.currentMessageIndex);
	data.currentMessageIndex = 0;
}

//THANKS TO COLIN BRADY FOR THIS LINK BELOW AND FOR SUGGESTING USING GETASYNCKEYSTATE INSTEAD OF SDL
//https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx
//https://blog.molecular-matters.com/2011/09/05/properly-handling-keyboard-input/ this link showed me the MapVirtualKey and MAPVK_VK_TO_CHAR, which let me cast  to char
void Lobby::updateState()
{
	//Not using mouse inputs for this app, but there are lots of things to check still

	if (data.enterServer)
	{
		data.enterServer = false;
		goToNextState(this);
	}

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

	if (mCurrentOption == ESTABLISHING_CONNECTION)
	{
		if (successfullyConnectedToServer)
		{
			goToNextState(this);
		}
		else
		{
			if (waitFrames > 0)
			{
				waitFrames--;
			}
			else
			{
				mCurrentOption = JOINING_SERVER_IP;
				lobbyOptionText = "Please enter the IP address, or just type \"default\"";
				waitFrames = 30;
			}
		}
	}

}

//I thought this was gonna be more elaborate. I'd have to make it so if this was a game, not a chat room. Or if it was a more active chatroom even
void Lobby::ClearScreen()
{
	system("CLS");
}

void Lobby::display()
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

	ClearScreen(); //clear tj """""""""buffer""""""""" yoshi https://www.youtube.com/watch?v=kpk2tdsPh0A creedit to pannen

	cout << data.headerMessage << endl << endl << endl;
	
	//gpGame->SetTextRed();
	cout << lobbyOptionText << endl << endl;
	//gpGame->SetTextDefault();

	for (int i = 9; i >= 0; i--)
	{
		cout << data.recentMessages[i] << endl;
	}

	cout << "LOBBY" << endl;

	//i put a '>' there cause it's like, CHAT HERE!!! haha it's good practice ok im very tired and hands r cold
	cout << ">" << data.currentChatMessage << "<\b";
	cout.flush();
}

//proof of concept, move the message up with "User: " at the beginning, then allow users to type a new one
void Lobby::PushMessageIntoQueue()
{
	for (int i = 9; i > 0; i--)
	{
		data.recentMessages[i] = data.recentMessages[i - 1];
	}

	data.recentMessages[0] = "User: " + data.currentChatMessage + "\n";
	clearCurrentMessage();
}

void Lobby::PushMessageIntoQueue(std::string newMessage)
{
	for (int i = 9; i > 0; i--)
	{
		data.recentMessages[i] = data.recentMessages[i - 1];
	}

	data.recentMessages[0] = newMessage + '\n';
	clearCurrentMessage();
}

void Lobby::processMessage()
{
	//PushMessageIntoQueue(); //verified! Not used in lobby, but will be useful in game chat (pushing to server, not the console!)
	if (data.myUsername == "")
	{
		bool canMoveForward = true;
		if (data.currentMessageIndex > 0)
		{
			for (int i = 0; i < data.currentMessageIndex; i++)
			{
				if (data.currentChatMessage[i] == ' ' || data.currentChatMessage[i] == '@' || data.currentChatMessage[i] == '#')
				{
					canMoveForward = false;
				}
			}
		}
		else
		{
			canMoveForward = false;
		}
		if (canMoveForward)
		{
			data.myUsername = data.currentChatMessage;
			PushMessageIntoQueue("Wonderful! Your name is now: " + data.currentChatMessage);
			//clearCurrentMessage();
			lobbyOptionText = "Type a number to choose one of the options below:\n(1) Start a Server\n(2) Join a Server\n(3) Quit Application"; //later I'd like a change username option if time permits
		}
		else
		{
			PushMessageIntoQueue("Sorry, that username is invalid. Please make sure there are no spaces, #, or @ signs!");
		}
	}
	else
	{
		//interpret their message
		//if it isn't applicable to an option from the list, discard it with a sorry message
		//otherwise, update the list accordingly

		if (data.currentMessageIndex > 0)
		{
			switch (mCurrentOption) //where are they in the menus?
			{
			case FIRST_SCREEN:
			{
				//just look at the first character. If they type 123 then it'll be option 1. Just for functionality for now, would like to improve if time permits
				switch (data.currentChatMessage[0])
				{
				case 49: //option 1, move to MAKING_SERVER
				{
					lobbyOptionText = "loading";
					PushMessageIntoQueue("You have opted to Create a New Server!");
					mCurrentOption = ESTABLISHING_CONNECTION;
					wantsToBeServer = true;
					mNetworkManager->initServer(data.portNumber);
					goToNextState(this);
				}
				break;
				case 50: //option 2, move to JOINING_SERVER
				{
					lobbyOptionText = "Please enter the IP address, or just type \"default\"";
					PushMessageIntoQueue("You have opted to Join an Existing Server!");
					mCurrentOption = JOINING_SERVER_IP;
					wantsToBeServer = false;
				}
				break;
				case 51: //option 3 say goodbye and exit
				{
					PushMessageIntoQueue("Sorry to see you go :(\n");
					printf("\n");
					gpGame->requestExit();
				}
				break;
				default: //not an option
				{
					PushMessageIntoQueue("Invalid Option. Please type a number that corresponds to an item in the list.");
				}
				break;
				}
			}
			break;
			case MAKING_SERVER:
			{
				bool canMoveForward = true;
				//make sure it's an int with less than 5 characters
				if (data.currentMessageIndex > 5)
				{
					canMoveForward = false;
				}
				for (int i = 0; i < data.currentMessageIndex; i++)
				{
					if (data.currentChatMessage[i] < 48 || data.currentChatMessage[i] > 57) //not a number
					{
						canMoveForward = false;
					}
				}
				if (canMoveForward)
				{
					data.portNumber = stoi(data.currentChatMessage);
					if (data.portNumber > 65535)
					{
						canMoveForward = false;
					}
				}
				if (canMoveForward)
				{
					PushMessageIntoQueue("Port Number set to: " + data.currentChatMessage);
					if (mNetworkManager->initServer(data.portNumber))
					{
						mCurrentOption = ESTABLISHING_CONNECTION;
						lobbyOptionText = "Establishing Connection: Please Wait";
						goToNextState(this);
					}
					else
					{
						PushMessageIntoQueue("Server could not be made at given Port Number. (May already exist?)");
					}
				}
				else
				{
					PushMessageIntoQueue("Invalid Option. Please type a whole number smaller than 65535 (no spaces).");
				}
			}
			break;
			case JOINING_SERVER:
			{
				bool canMoveForward = true;
				//make sure it's an int with less than 5 characters
				if (data.currentMessageIndex > 5)
				{
					canMoveForward = false;
				}
				for (int i = 0; i < data.currentMessageIndex; i++)
				{
					if (data.currentChatMessage[i] < 48 || data.currentChatMessage[i] > 57) //not a number
					{
						canMoveForward = false;
					}
				}
				if (canMoveForward)
				{
					data.portNumber = stoi(data.currentChatMessage);
					if (data.portNumber > 65535)
					{
						canMoveForward = false;
					}
				}
				if (canMoveForward)
				{
					PushMessageIntoQueue("Port Number set to: " + data.currentChatMessage);
					mCurrentOption = JOINING_SERVER_IP;
					lobbyOptionText = "Please enter the IP address, or just type \"default\"";

				}
				else
				{
					PushMessageIntoQueue("Invalid Option. Please type a whole number smaller than 65535 (no spaces).");
				}
			}
			break;
			case JOINING_SERVER_IP:
			{
				bool canMoveForward = true;
				
				if (data.currentChatMessage == "Default" || data.currentChatMessage == "default")
				{
					data.ipAddress = "127.0.0.1";
				}
				else
				{					
					canMoveForward = false;
				}

				if (canMoveForward)
				{
					data.doesUpdateNetworking = 1;
					mCurrentOption = ESTABLISHING_CONNECTION;
					lobbyOptionText = "Establishing Connection: Please Wait";

					mNetworkManager->initClient(data.portNumber, data.ipAddress);

					//mNetworkManager->updateServer();

					//RakPeerInterface *tempPeer = mNetworkManager->mpPeer;
					//Packet *tempPacket = mNetworkManager->mpPacket;

					//for (tempPacket = tempPeer->Receive(); tempPacket; tempPeer->DeallocatePacket(tempPacket), tempPeer->Receive())
					//{
					//	switch (tempPacket->data[0])
					//	{
					//	case ID_CONNECTION_REQUEST_ACCEPTED:
					//	{
					//		// L O L?
					//	}
					//	break;
					//	default:
					//	{
					//		// bad
					//	}
					//	break;
					//	}
					//}


					/*if (mNetworkManager->initClient(data.portNumber, data.ipAddress))
					{
						goToNextState(this);
					}
					else
						PushMessageIntoQueue("Cannot access server with the given IP / Port Number");*/
				}
				else
				{
					PushMessageIntoQueue("Invalid IP address. You can type \"default\" if a local server is active");
				}
			}
			break;			
			default:
			{

			}
			break;
			}
		}

	}


	data.doesDisplay = 1;
}

void Lobby::goToNextState(ApplicationState *passData)
{
	//dan this is good use of polymorphism i think -robby
	gpGame->theState = gpGame->theGame;
	next = gpGame->theState;
	next->onArriveFromPrevious(passData);
}

void Lobby::updateNetworking()
{
	if (!data.doesUpdateNetworking)
		return;

	mNetworkManager->updateServer();

	/*for (mNetworkManager->mpPacket = mNetworkManager->mpPeer->Receive(); mNetworkManager->mpPacket; mNetworkManager->mpPeer->DeallocatePacket(mNetworkManager->mpPacket), mNetworkManager->mpPeer->Receive())
	{
		switch (mNetworkManager->mpPacket->data[0])
		{
		case ID_CONNECTION_REQUEST_ACCEPTED:
		{
			successfullyConnectedToServer = true;
			data.doesUpdateNetworking = 0;
		}
		break;
		case ID_CONNECTION_ATTEMPT_FAILED:
		{
			printf("suthi");
		}
		break;
		default:
		{
			printf("\nnot working");
		}
		break;
		}

		int ass = 8;
	}*/

	//successfullyConnectedToServer = 

}