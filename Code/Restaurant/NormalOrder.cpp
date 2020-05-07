#include "NormalOrder.h"

NormalOrder::NormalOrder(int ID, int size, int time, double money, int autoP) :
	Order(ID, TYPE_NRM, size, time, money)
{
	AutoPromote = autoP;
}