#pragma once
#include "Rest/Order.h"
class NormalOrder : public Order
{
	int AutoPromote;

public:
	NormalOrder(int ID, ORD_TYPE type, int size, int time, double money, int autoP);
};

