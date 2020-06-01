#pragma once
#include "Rest/Order.h"
#include "NormalOrder.h"

class VipOrder : public Order
{
	double PriorityFactor;
	bool isUrgent;
	int PromotionTime;   //to count the time correctly for the promoted normal orders
	static int Vip_WT;

	void CalculatePriorityFactor();

public:
	VipOrder(int ID, int size, int time, double money);

	VipOrder(NormalOrder* ord,int time);   //conversion constructor for promoted normal orders

	static void SetVipWT(int time);

	float GetPriorityFactor();

	bool IsUrgent();
	void Urgent();

	bool operator > (VipOrder& ord);
	operator int();
};

