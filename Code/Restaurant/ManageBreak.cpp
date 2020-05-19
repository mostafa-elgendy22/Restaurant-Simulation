#include "ManageBreak.h"

ManageBreak::ManageBreak(Cook* cook)
{
	pCook = cook;
}

void ManageBreak::EndBreak(Restaurant* pRest)
{
	pCook->ResetServicedOrders();
	pCook->SetStartBreakTime(-1);
	pRest->AddToCookList(pCook);
}

void ManageBreak::EndRest(Restaurant* pRest)
{
	pCook->SetInjury(false);
	pCook->DoubleSpeed();
	pCook->SetStartBreakTime(-1);
	if (pCook->GetServicedOrders() == pRest->GetMaxNumberOrders())
	{
		pRest->AddToBreakList(pCook);
	}
	else
	{
		pRest->AddToCookList(pCook);
	}
}