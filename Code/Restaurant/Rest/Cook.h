#pragma once

#include "..\Defs.h"
class Restaurant;

class Cook
{
	int ID;                    //the id of the cook
	ORD_TYPE type;	           //cook type
	int speed;		           //dishes that cook can prepare in one clock tick (in one timestep)
	int BreakDuration;         //the number of timesteps of the cook's break
	int ServicedOrders;        //number of serviced orders by the cook
	int StartBreakTime;        //the time step at which the cook started his break
	bool isInjured;            //a flag to determine if the cook is injured
	static int RstPrd;         //the rest period of all cooks
	static int maxNumOrders;   // the max number of orders that a cook can prepare before taking a break

public:

	Cook(int r_id, ORD_TYPE r_type, int r_speed, int Break);
	~Cook();

	void setType(ORD_TYPE);
	ORD_TYPE GetType() const;

	void setID(int);
	int GetID() const;

	static void SetRestPeriod(int time);
	static int GetRestPeriod();

	static void SetMaxNumOrders(int number);
	static int GetMaxNumberOrders();


	int GetSpeed() const;
	void HalfSpeed();

	void IncrementServicedOrders();
	int GetServicedOrders();
	void ResetServicedOrders();  // to be used after the cook ends his break

	int GetStartBreakTime();
	void SetStartBreakTime(int time);
	int GetBreakDuration();

	bool operator > (Cook& cook);

	void SetInjury(bool flag);
	bool IsInjured();

	void EndBreak(Restaurant* pRest);
	void EndRest(Restaurant* pRest);
};