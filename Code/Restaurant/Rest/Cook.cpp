#include "Cook.h"
#include "Restaurant.h"

int Cook::maxNumOrders = 0;
int Cook::RstPrd = 0;


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

void Cook::SetRestPeriod(int time)
{
	RstPrd = time;
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
	if (!speed)
		speed = 1;
}

void Cook::IncrementServicedOrders()
{
	ServicedOrders++;
}

int Cook::GetBreakDuration()
{
	return BreakDuration;
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

bool Cook::operator > (Cook& cook)
{
	return (StartBreakTime + BreakDuration) < (cook.StartBreakTime + cook.BreakDuration);
}

void Cook::SetMaxNumOrders(int number)
{
	maxNumOrders = number;
}

int Cook::GetMaxNumberOrders()
{
	return maxNumOrders;
}

int Cook::GetRestPeriod()
{
	return RstPrd;
}

void Cook::EndBreak(Restaurant* pRest)
{
	ServicedOrders = 0;
	StartBreakTime = -1;
	pRest->AddToCookList(this);
}

void Cook::EndRest(Restaurant* pRest)
{
	isInjured = false;
	speed = speed * 2;
	StartBreakTime = -1;
	pRest->AddToCookList(this);
}