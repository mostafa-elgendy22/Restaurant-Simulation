#pragma once
#include"Events/Event.h"

class CancellationEvent : public Event
{
public:

	CancellationEvent(int eTime, int oID);

	virtual void Execute(Restaurant* pRest);	//overrided execute function

};

