#include "ArrivalEvent.h"

#include "..\Rest\Restaurant.h"

ArrivalEvent::ArrivalEvent(int eTime, int oID, ORD_TYPE oType, double oMoney, int size) :Event(eTime, oID)

{
	OrdType = oType;
	OrdMoney = oMoney;
	OrdSize = size;
}

void ArrivalEvent::Execute(Restaurant* pRest)

{
	//This function should create an order and fills its data 
	// Then adds it to normal, vegan, or VIP order lists that you will create in phase1

	///For the sake of demo, this function will just create an order and add it to DemoQueue
	///Remove the next code lines in phases 1&2
	Order* pOrd = new Order(OrderID, OrdType, OrdSize, EventTime, OrdMoney);

	switch (OrdType)
	{

	case TYPE_NRM:
		pRest->addtonormarlist(pOrd);
		break;
	case TYPE_VIP:
		pRest->addtoviplist(pOrd);
		break;
	case TYPE_VGAN:
		pRest->addtoveganlist(pOrd);
		break;
	}

}
