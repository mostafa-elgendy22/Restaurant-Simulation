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
