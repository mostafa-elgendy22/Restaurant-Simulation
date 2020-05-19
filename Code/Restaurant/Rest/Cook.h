#pragma once

#include "..\Defs.h"

class Cook
{
	int ID;
	ORD_TYPE type;	//cook type
	int speed;		//dishes that cook can prepare in one clock tick (in one timestep)
	int BreakDuration;
	int ServicedOrders;       //number of serviced orders by the cook
	int StartBreakTime;      //the time step at which the cook started his break
	bool isInjured;


public:

	Cook(int r_id, ORD_TYPE r_type, int r_speed, int Break);
	~Cook();
	ORD_TYPE GetType() const;
	int GetID() const;
	void setID(int);
	void setType(ORD_TYPE);
	int GetSpeed() const;

	void HalfSpeed();
	void DoubleSpeed();

	void IncrementServicedOrders();
	int GetServicedOrders();
	void ResetServicedOrders();  // to be used after the cook ends his break

	int GetStartBreakTime();
	void SetStartBreakTime(int time);

	operator int();

	void SetInjury(bool flag);
	bool IsInjured();
};
