#pragma once
#include "Rest/Order.h"
class NormalOrder : public Order
{

public:
	NormalOrder(int ID, int size, int time, double money);

	operator int();
};

