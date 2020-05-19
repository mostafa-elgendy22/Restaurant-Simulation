#include "CancellationEvent.h"

#include"..\Restaurant\Rest\Restaurant.h"

CancellationEvent::CancellationEvent(int eTime, int oID) :Event(eTime, oID)
{
}

void CancellationEvent::Execute(Restaurant* pRest)	//overrided execute function
{
	pRest->CancelOrder(this->OrderID);
}