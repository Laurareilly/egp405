#include <iostream>
#include <stdio.h>
#include "../../../DevSDKs/DevSDKs/include/RakNet/RakPeerInterface.h"

using namespace std;
using namespace RakNet;

int main()
{
	unsigned int maxClients = 10;
	unsigned short serverPort = 60000;
	char str[512];
	RakNet::RakPeerInterface *peer = RakNet::RakPeerInterface::GetInstance();
	bool isServer;

	cout << "(C) or (S)erver?\n";
	cin >> str;

	cout << "Enter the server port number\n";
	cin >> serverPort;

	if (str[0] == 'c' || str[0] == 'C') //if we're starting a client
	{
		RakNet::SocketDescriptor sd;
		peer->Startup(1, &sd, 1);
		isServer = false;
	}
	else //if we're starting a server
	{
		cout << "Enter the max amount of clients\n";
		cin >> maxClients;
		RakNet::SocketDescriptor sd(serverPort, 0);
		peer->Startup(maxClients, &sd, 1);
		isServer = true;
	}

	RakNet::RakPeerInterface::DestroyInstance(peer);

	cin.get();
	cin.ignore();
	return 0;
}