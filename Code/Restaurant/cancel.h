#pragma once
#include"Events/Event.h"

class cancel : public Event

{


public:

	cancel(int eTime, int oID);

	virtual void Execute(Restaurant* pRest);	//overrided execute function



};

