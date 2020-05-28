#pragma once
#include "Rest/Order.h"
#include "NormalOrder.h"

class VipOrder : public Order
{
	float PriorityFactor;
	bool isUrgent;
	static int Vip_WT;

	void CalculatePriorityFactor();

public:
	VipOrder(int ID, int size, int time, double money);

	VipOrder(NormalOrder* ord);

	static void SetVipWT(int time);

	float GetPriorityFactor();

	bool IsUrgent();

	bool operator > (VipOrder& ord);
	operator int();
};

