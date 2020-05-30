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
	ord->SetFT(ServiceTime + startTime);
	if (dynamic_cast<VipOrder*> (ord))
	{
		VipOrder* pVip = dynamic_cast<VipOrder*> (ord);
		if (pVip->IsUrgent())
		{
			if (cook->GetStartBreakTime() != -1) // the cook was in break or rest
			{
				if (!(cook->IsInjured()))       // the cook was in break
				{
					cook->ResetServicedOrders();
				}
			}
		}
	}
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
	if (pVip && pVip->IsUrgent())
	{
		if (cook->GetServicedOrders() == Cook::GetMaxNumberOrders())
		{
			cook->ResetServicedOrders();
		}
		pRest->AddToCookList(cook);
	}
	else if (cook->IsInjured())
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
