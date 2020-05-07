#pragma once

#include "..\Defs.h"

class Cook
{
	int ID;
	ORD_TYPE type;	//cook type
	int speed;		//dishes that cook can prepare in one clock tick (in one timestep)
	int BreakDuration;

public:

	Cook(int r_id, ORD_TYPE r_type, int r_speed, int Break);
	virtual ~Cook();
	int GetID() const;
	ORD_TYPE GetType() const;
	void setID(int);
	void setType(ORD_TYPE);
	int GetSpeed() const;
	void HalfSpeed();
};
