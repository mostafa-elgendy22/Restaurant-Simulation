#pragma once

#include "..\Defs.h"
class Restaurant;

class Cook
{
	int ID;
	ORD_TYPE type;	//cook type
	int speed;		//dishes that cook can prepare in one clock tick (in one timestep)
	int BreakDuration;
	int ServicedOrders;       //number of serviced orders by the cook
	int StartBreakTime;      //the time step at which the cook started his break
	bool isInjured;
	static int RstPrd;
	static int maxNumOrders;  // the max number of orders that a cook can prepare before taking a break

public:

	Cook(int r_id, ORD_TYPE r_type, int r_speed, int Break);
	~Cook();
	ORD_TYPE GetType() const;
	int GetID() const;
	static void SetRestPeriod(int time);
	static int GetRestPeriod();
	static void SetMaxNumOrders(int number);
	static int GetMaxNumberOrders();
	void setID(int);
	void setType(ORD_TYPE);
	int GetSpeed() const;

	void HalfSpeed();

	void IncrementServicedOrders();
	int GetServicedOrders();
	void ResetServicedOrders();  // to be used after the cook ends his break

	int GetStartBreakTime();
	void SetStartBreakTime(int time);

	operator int();

	void SetInjury(bool flag);
	bool IsInjured();
	
	int GetBreakDuration();

	void EndBreak(Restaurant* pRest);
	void EndRest(Restaurant* pRest);
};
