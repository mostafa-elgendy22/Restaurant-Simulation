#include "PromotionEvent.h"
#include"..\Restaurant\Rest\Restaurant.h"

PromotionEvent::PromotionEvent(int eTime, int oID, int r_money) :Event(eTime, oID)
{
	money = r_money;
}

void PromotionEvent::Execute(Restaurant* pRest)
{
	pRest->PromoteOrder(OrderID);
}