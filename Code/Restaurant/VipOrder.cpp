#include "VipOrder.h"


int VipOrder::Vip_WT = 0;

VipOrder::VipOrder(int ID, int size, int time, double money) :
	Order(ID, TYPE_VIP, size, time, money)
{
	CalculatePriorityFactor();
	isUrgent = false;
	PromotionTime = ArrTime;
}

VipOrder::VipOrder(NormalOrder* ord, int time) :
	Order(ord->GetID(), TYPE_VIP, ord->GetSize(), ord->GetAT(), ord->GetMoney())
{
	CalculatePriorityFactor();
	isUrgent = false;
	PromotionTime = time;
}

void VipOrder::CalculatePriorityFactor()
{
	int money_weight, size_weight, time_weight;

	money_weight = 5;
	size_weight = -5;
	time_weight = -10;

	PriorityFactor = money_weight * totalMoney + size_weight * Size + time_weight * ArrTime;
}

float VipOrder::GetPriorityFactor()
{
	return PriorityFactor;
}

bool VipOrder::IsUrgent()
{
	return isUrgent;
}

void VipOrder::Urgent()
{
	isUrgent = 1;
}


bool VipOrder::operator > (VipOrder& ord)
{
	return PriorityFactor > ord.PriorityFactor;
}

VipOrder::operator int()
{
	return PromotionTime + Vip_WT;
}

void VipOrder::SetVipWT(int time)
{
	Vip_WT = time;
}