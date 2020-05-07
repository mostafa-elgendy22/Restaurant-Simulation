#pragma once
#include "Rest/Order.h"
#include "NormalOrder.h"

class VipOrder : public Order
{
	float PriorityFactor;

public:
	VipOrder(int ID, int size, int time, double money);

	VipOrder(NormalOrder* ord);

	void CalculatePriorityFactor();

	float GetPriorityFactor();
};

