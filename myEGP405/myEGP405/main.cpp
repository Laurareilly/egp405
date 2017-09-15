
#include <stdio.h>
#include "Raknet/RakNetTypes.h"  // MessageID
#include <string.h>
#include "GameMessage.h"
#include <iostream>
#include <string>
#include "Game.h"


using namespace std;
Game *gpGame;


char str[512];


//UsernameMessage connectedUserList[10];
int connectedUserCount = 10;

//preset colors for specific messages
//char red[] = {}
//char purple[]
//char black[]
//char white[]
char blue[] = { 0x1b, '[', '1', ';', '3', '4', 'm', 0 };

int gDefaultColors;

//http://www.cplusplus.com/forum/beginner/1640/
WORD GetConsoleTextAttribute(HANDLE hCon)
{
	CONSOLE_SCREEN_BUFFER_INFO con_info;
	GetConsoleScreenBufferInfo(hCon, &con_info);
	return con_info.wAttributes;
}

void SetTextRed()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED);
}

void SetTextPurple()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_INTENSITY | FOREGROUND_RED | FOREGROUND_BLUE);
}

void SetTextDefault()
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), gDefaultColors);
}

int main(void)
{
	gpGame = new Game();
	gpGame->processLoop();


	//gDefaultColors = GetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE));
	//char myUsernameString[31];

	//SetTextPurple();


	return 0;
}