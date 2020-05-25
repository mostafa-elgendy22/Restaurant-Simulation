#pragma once
#include "Rest/Order.h"
class NormalOrder : public Order
{
	static int AutoPromote;

public:

	NormalOrder(int ID, int size, int time, double money);

	static void SetAutoPromote(int time);

	static int GetAutoPromote();

	operator int();
};

