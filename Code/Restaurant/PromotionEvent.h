#pragma once
#include"Events/Event.h"

class PromotionEvent : public Event
{

	double ExMoney;
public:

	PromotionEvent(int eTime, int oID, int r_money);

	virtual void Execute(Restaurant* pRest);	//overrided execute function
};
