#pragma once

#include "RakNet\RakPeerInterface.h"

using namespace RakNet;

class NetworkManager
{
public:
	NetworkManager();
	~NetworkManager();

	void initializeNetwork();

	void initServer();
	void initClient();
	void updateServer();
	void updateClient();

private:

	RakPeerInterface *mpPeer;
	Packet *mpPacket;

	unsigned int maxClients = 10;
	unsigned int serverPort = 1111;
	bool mIsServer;
};