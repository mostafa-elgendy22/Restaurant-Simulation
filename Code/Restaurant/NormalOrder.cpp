#include "NormalOrder.h"

NormalOrder::NormalOrder(int ID, int size, int time, double money) :
	Order(ID, TYPE_NRM, size, time, money)
{

}

NormalOrder::operator int()
{
	return ID;
}