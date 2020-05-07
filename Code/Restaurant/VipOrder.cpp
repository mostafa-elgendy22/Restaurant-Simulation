#include "VipOrder.h"


VipOrder::VipOrder(int ID, int size, int time, double money) :
	Order(ID, TYPE_VIP, size, time, money)
{
	CalculatePriorityFactor();
}

VipOrder::VipOrder(NormalOrder* ord) :
	Order(ord->GetID(), TYPE_VIP, ord->Getsize(), ord->GetAT(), ord->GetMoney())
{
	CalculatePriorityFactor();
}

void VipOrder::CalculatePriorityFactor()
{
	//write priority equation here

	//PriorityFactor = ;
}

float VipOrder::GetPriorityFactor()
{
	return PriorityFactor;
}