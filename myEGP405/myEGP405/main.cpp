
#include <stdio.h>
#include "Raknet/RakNetTypes.h"  // MessageID
#include <string.h>
#include "GameMessage.h"
#include <iostream>
#include <string>
#include "Game.h"
#include "commonFile.h"


using namespace std;
Game *gpGame;


char str[512];

//string butt but butbbut but btue
//UsernameMessage connectedUserList[10];
int connectedUserCount = 10;

//preset colors for specific messages
//char red[] = {}
//char purple[]
//char black[]
//char white[]
char blue[] = { 0x1b, '[', '1', ';', '3', '4', 'm', 0 };



int main(void)
{
	gpGame = new Game();
	gpGame->processLoop();

	system("Pause");
	//gDefaultColors = GetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE));
	//char myUsernameString[31];

	//SetTextPurple();


	return 0;
}