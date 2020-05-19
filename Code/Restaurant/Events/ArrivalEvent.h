#ifndef __ARRIVAL_EVENT_H_
#define __ARRIVAL_EVENT_H_

#include "Event.h"



class ArrivalEvent : public Event
{
	int OrdSize;	      //order size
	ORD_TYPE OrdType;    //order type: Normal, vegan, VIP	                
	double OrdMoney;	//Total order money


public:

	ArrivalEvent(int eTime, int oID, ORD_TYPE oType, double oMoney, int size);


	virtual void Execute(Restaurant* pRest);	//override execute function

};

#endif