#include "OrderService.h"
#include "Rest/Restaurant.h"
#include <cmath> //to use the ceil function

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
	ord->SetST(ServiceTime);
	ord->SetFT(ServiceTime + startTime);
	pRest->AddToInserviceList(this);
}

void OrderService::InjureCook(int currentTimeStep)
{
	cook->SetInjury(true);
	int remainedDishes = ord->GetSize() - ((currentTimeStep - startTime) * cook->GetSpeed());
	cook->HalfSpeed();
	ServiceTime = (currentTimeStep - startTime) + int(ceil(float(remainedDishes) / cook->GetSpeed()));
	ord->SetST(ServiceTime);
	ord->SetFT(ServiceTime + startTime);
}

void OrderService::FinishOrder(Restaurant* pRest, int time)
{
	cook->IncrementServicedOrders();
	ord->SetStatus(DONE);
	pRest->AddToFinishedList(ord);

	VipOrder* pVip = dynamic_cast<VipOrder*> (ord);

	if (pVip && pVip->IsUrgent() && cook->GetStartBreakTime() != -1)  //the order is vip urgent order and the cook was in break or rest
	{
		if (cook->GetServicedOrders() > Cook::GetMaxNumberOrders())
		{
			cook->ResetServicedOrders();
		}
		pRest->AddToCookList(cook);
		return;
	}

	if (cook->IsInjured())
	{
		if (cook->GetServicedOrders() == Cook::GetMaxNumberOrders())
		{
			cook->ResetServicedOrders();
		}
		cook->SetStartBreakTime(time);
		pRest->AddToRestList(cook);
	}
	else if (cook->GetServicedOrders() == Cook::GetMaxNumberOrders())
	{
		cook->SetStartBreakTime(time);
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
