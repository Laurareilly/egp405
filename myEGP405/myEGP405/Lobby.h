
#ifndef  _LOBBYSTATE_H
#define  _LOBBYSTATE_H

#include "ApplicationState.h"

class Lobby : public ApplicationState
{
public:

	virtual void updateInput()
	{

	}

	virtual void updateNetworking()
	{

	}

	virtual void updateState()
	{
		//trigger transition
		goToNextState(&data);
	}

	virtual void display()
	{

	}

	virtual void onArriveFromPrevious(ApplicationState *passData)
	{

	}

	virtual void goToNextState(ApplicationState *passData)
	{
		next->onArriveFromPrevious(passData);
	}

};


#endif