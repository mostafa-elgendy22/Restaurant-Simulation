#pragma once

#include "Rest/Cook.h"
#include "Rest/Order.h"


class ServeOrder
{
	Order* order;
	Cook* cook;
	bool isFinished;

public:

	ServeOrder(Order* o, Cook* c);
	bool isServiced();
	void Serve();
};

