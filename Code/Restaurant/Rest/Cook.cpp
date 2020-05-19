#include "Cook.h"


Cook::Cook(int r_id, ORD_TYPE r_type, int r_speed, int Break)
{
	ID = r_id;
	type = r_type;
	speed = r_speed;
	BreakDuration = Break;
	ServicedOrders = 0;
	isInjured = false;
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

int Cook::GetSpeed() const
{
	return speed;
}

void Cook::HalfSpeed()
{
	speed = speed / 2;
}

void Cook::DoubleSpeed()
{
	speed = speed * 2;
}

void Cook::IncrementServicedOrders()
{
	ServicedOrders++;
}

int Cook::GetServicedOrders()
{
	return ServicedOrders;
}

void Cook::ResetServicedOrders()
{
	ServicedOrders = 0;
}

int Cook::GetStartBreakTime()
{
	return StartBreakTime;
}
void Cook::SetStartBreakTime(int time)
{
	StartBreakTime = time;
}

void Cook::SetInjury(bool flag)
{
	isInjured = flag;
}
bool Cook::IsInjured()
{
	return isInjured;
}

Cook::operator int()
{
	return (StartBreakTime + BreakDuration);
}