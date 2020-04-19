#include "NormalOrder.h"

NormalOrder::NormalOrder(int ID, ORD_TYPE type, int size, int time, double money, int autoP) :
	Order(ID, type, size, time, money)
{
	AutoPromote = autoP;
}