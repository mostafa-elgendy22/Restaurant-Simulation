#include "NormalOrder.h"

int NormalOrder::AutoPromote = 0;

NormalOrder::NormalOrder(int ID, int size, int time, double money) :
	Order(ID, TYPE_NRM, size, time, money)
{

}

NormalOrder::operator int()
{
	return ID;
}

void NormalOrder::SetAutoPromote(int time)
{
	AutoPromote = time;
}

int NormalOrder::GetAutoPromote()
{
	return AutoPromote;
}