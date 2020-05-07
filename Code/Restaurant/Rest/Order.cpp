#include "Order.h"

Order::Order(int id, ORD_TYPE r_Type, int size, int time, double money)

{
	ID = (id > 0 && id < 1000) ? id : 0;	//1<ID<999

	type = r_Type;

	status = WAIT;

	Size = size;

	totalMoney = money;

	ArrTime = time;
}

Order::~Order()
{

}

int Order::GetID()

{
	return ID;
}

ORD_TYPE Order::GetType() const

{
	return type;
}

void Order::Setsize(int d)
{
	Size = d > 0 ? d : 0;
}

int Order::Getsize() const
{
	return Size;
}

void Order::setStatus(ORD_STATUS s)
{
	status = s;
}

void Order::SetType(ORD_TYPE typ)
{
	type = typ;
}

ORD_STATUS Order::getStatus() const
{
	return status;
}

bool Order::operator > (const Order& x)const
{
	return (totalMoney > x.totalMoney);
}

int Order::GetAT()
{
	return ArrTime;
}

int Order::GetST()
{
	return ServTime;
}

int Order::GetFT()
{
	return FinishTime;
}

bool Order::operator == (const Order*& ord)
{
	return ID == ord->ID;
}

Order::operator int() const
{
	return ID;
}

double Order::GetMoney()
{
	return totalMoney;
}