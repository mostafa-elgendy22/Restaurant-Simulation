#pragma once
#include "Rest/Order.h"
class VeganOrder : public Order
{

public:
	VeganOrder(int ID, int size, int time, double money);
};

