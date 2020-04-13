#include "cancel.h"

#include"..\Restaurant\Rest\Restaurant.h"

cancel::cancel(int eTime, int oID):Event(eTime,oID)
{}

void cancel::Execute(Restaurant* pRest)	//overrided execute function
{
	pRest->cancelorder(this->getOrderID());
}