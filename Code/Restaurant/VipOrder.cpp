#include "VipOrder.h"


VipOrder::VipOrder(int ID, ORD_TYPE type, int size, int time, double money) :
	Order(ID, type, size, time, money)
{
	CalculatePriorityFactor();
}


void VipOrder::CalculatePriorityFactor()
{
	//write priority equation here

	//PriorityFactor = ;
}