#include "PromotionEvent.h"


PromotionEvent::PromotionEvent(int eTime, int oID, double money) :Event(eTime, oID)
{
	ExMoney = money;
}

void PromotionEvent::Execute(Restaurant* pRest)
{

}