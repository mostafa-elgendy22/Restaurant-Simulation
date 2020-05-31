#include "PromotionEvent.h"
#include"..\Restaurant\Rest\Restaurant.h"

PromotionEvent::PromotionEvent(int eTime, int oID, int r_money) :Event(eTime, oID)
{
	ExMoney = r_money;
}

void PromotionEvent::Execute(Restaurant* pRest)
{
	NormalOrder* pOrd = pRest->GetNormalOrderFromID(OrderID);
	if (pOrd)
	{
		pOrd->SetMoney(pOrd->GetMoney() + ExMoney);
		pRest->PromoteOrder(OrderID, EventTime);
	}
}