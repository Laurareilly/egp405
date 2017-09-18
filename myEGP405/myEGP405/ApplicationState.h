//base class for state machine

#ifndef  _APPLICATIONSTATE_H
#define  _APPLICATIONSTATE_H

class ApplicationState abstract //this type of state can never be used
{
public:
	virtual void updateInput() = 0;
	virtual void updateNetworking() = 0;
	virtual void updateState() = 0;
	virtual void display() = 0;
protected:
	ApplicationState *next, *previous;

	struct ApplicationStateData
	{
		char doesUpdateInput;
		char doesUpdateNetworking;
		char doesUpdateState;
		char doesDisplay;
	} data;

	virtual void goToNextState(ApplicationState *passData) = 0;
	virtual void onArriveFromPrevious(ApplicationState *passData) = 0;
};



#endif // ! _APPLICATIONSTATE_H
