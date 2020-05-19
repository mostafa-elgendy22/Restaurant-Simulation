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
	switch (OrdType)
	{
		NormalOrder* ord1;
		VipOrder* ord2;
		VeganOrder* ord3;


	case TYPE_NRM:
		ord1 = new NormalOrder(OrderID, OrdSize, EventTime, OrdMoney);
		pRest->AddToNormalList(ord1);
		break;

	case TYPE_VIP:
		ord2 = new VipOrder(OrderID,  OrdSize, EventTime, OrdMoney);
		pRest->AddToVipList(ord2);
		break;

	case TYPE_VGAN:
		ord3 = new VeganOrder(OrderID, OrdSize, EventTime, OrdMoney);
		pRest->AddToVeganList(ord3);
		break;
	}
}