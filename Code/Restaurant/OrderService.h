#pragma once


#include "Rest/Cook.h"
#include "Rest/Order.h"
class Restaurant;

class OrderService
{

	Cook* cook;       //the in_service cook
	Order* ord;       //the in_service order
	int ServiceTime;   //the calculated service time
	int startTime;    //start time of the order service

public:

	OrderService(Order* o, Cook* c, int time);
	void Serve(Restaurant* pRest);
	void InjureCook(int currentTimeStep);
	void FinishOrder(Restaurant* pRest, int time);
	operator int();
	operator bool();
	Order* GetOrder();
	Cook* GetCook();
	int GetStartTime();
};

