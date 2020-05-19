#pragma once
#include "Rest/Order.h"
#include "NormalOrder.h"

class VipOrder : public Order
{
	float PriorityFactor;
	bool isUrgent;


	void CalculatePriorityFactor();

public:
	VipOrder(int ID, int size, int time, double money);

	VipOrder(NormalOrder* ord);

	float GetPriorityFactor();

	bool IsUrgent();

	bool operator > (VipOrder* ord);
};

