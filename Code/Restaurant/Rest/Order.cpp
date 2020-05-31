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

void Order::SetSize(int s)
{
	Size = s > 0 ? s : 0;
}

int Order::GetSize() const
{
	return Size;
}

void Order::SetStatus(ORD_STATUS s)
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

double Order::GetMoney()
{
	return totalMoney;
}

bool Order::operator > (Order& ord)
{
	if (ord.FinishTime == this->FinishTime)
	{
		return (ord.ServTime > this->ServTime);
	}

	return ord.FinishTime > this->FinishTime;

}

void Order::SetST(int time)
{
	ServTime = time;
}

void Order::SetFT(int time)
{
	FinishTime = time;
}

void Order::SetMoney(double money)
{
	totalMoney = money;
}