#pragma once
#include "Rest/Order.h"
class VeganOrder : public Order
{

public:
	VeganOrder(int ID, ORD_TYPE type, int size, int time, double money);
};

