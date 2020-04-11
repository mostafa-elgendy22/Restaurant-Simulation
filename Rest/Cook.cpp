#include "Cook.h"


Cook::Cook()
{
}


Cook::Cook(int r_id, ORD_TYPE r_type, int r_speed)
{
	ID = r_id;
	type = r_type;
	speed = r_speed;
}

Cook::~Cook()
{
}


int Cook::GetID() const
{
	return ID;
}


ORD_TYPE Cook::GetType() const
{
	return type;
}


void Cook::setID(int id)
{
	ID = id;
}

void Cook::setType(ORD_TYPE t)
{
	type = t;
}


