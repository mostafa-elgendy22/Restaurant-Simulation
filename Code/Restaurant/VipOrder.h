#pragma once
#include "Rest/Order.h"
class VipOrder : public Order
{
	float PriorityFactor;

public:
	VipOrder(int ID, ORD_TYPE type, int size, int time, double money);

	void CalculatePriorityFactor();
};

