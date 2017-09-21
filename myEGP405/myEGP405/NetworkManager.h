#pragma once

#include "RakNet\RakPeerInterface.h"
#include <string>

using namespace RakNet;

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();

	void initializeNetwork();

	bool initServer(int cPort);
	bool initClient(int cPort, char* cIP);
	void updateServer();
	void updateClient();

private:

	RakPeerInterface *mpPeer;
	Packet *mpPacket;

	unsigned int maxClients = 10;
	unsigned int serverPort = 1111;
	bool mIsServer;
};