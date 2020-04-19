#pragma once

#include "..\Defs.h"

#pragma once
class Cook
{
	int ID;
	ORD_TYPE type;	//cook type
	int speed;		//dishes that cook can prepare in one clock tick (in one timestep)
	int maxNumCooks; //number of orders a cook must prepare before taking a break
	int BreakDuration;


public:

	Cook(int r_id, ORD_TYPE r_type, int r_speed,int max,int Break);
	virtual ~Cook();
	int GetID() const;
	ORD_TYPE GetType() const;
	void setID(int);
	void setType(ORD_TYPE);
	int GetSpeed() const;
};
