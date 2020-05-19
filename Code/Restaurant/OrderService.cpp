#include "OrderService.h"
#include "Rest/Restaurant.h"
#include <cmath>

OrderService::OrderService(Order* o, Cook* c, int time)
{
	ord = o;
	cook = c;
	ServiceTime = int(ceil(float(ord->GetSize()) / cook->GetSpeed()));
	startTime = time;
}


void OrderService::Serve(Restaurant* pRest)
{
	ord->SetStatus(SRV);
	pRest->AddToInserviceList(this);
	ord->SetST(ServiceTime);
	ord->SetFT(ServiceTime + ord->GetAT());
}

void OrderService::InjureCook(int currentTimeStep)
{
	cook->SetInjury(true);
	int remainedDishes = ord->GetSize() - ((currentTimeStep - startTime) * cook->GetSpeed());
	cook->HalfSpeed();
	ServiceTime = (currentTimeStep - startTime) + int(ceil(float(remainedDishes) / cook->GetSpeed()));
	ord->SetST(ServiceTime);
	ord->SetFT(ServiceTime + ord->GetAT());
}

void OrderService::FinishOrder(Restaurant* pRest, int time)
{
	cook->IncrementServicedOrders();
	ord->SetStatus(DONE);
	ord->SetFT(time);
	pRest->AddToFinishedList(ord);
	if (cook->IsInjured())
	{
		pRest->AddToRestList(cook);
	}
	else if (cook->GetServicedOrders() == pRest->GetMaxNumberOrders())
	{
		pRest->AddToBreakList(cook);
	}
	else
	{
		pRest->AddToCookList(cook);
	}
}

OrderService::operator int()
{
	return ord->GetFT();
}

OrderService::operator bool()
{
	return !cook->IsInjured();
}

Order* OrderService::GetOrder()
{
	return ord;
}

Cook* OrderService::GetCook()
{
	return cook;
}


int OrderService::GetStartTime()
{
	return startTime;
}
