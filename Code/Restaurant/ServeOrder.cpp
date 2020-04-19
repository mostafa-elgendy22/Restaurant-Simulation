#include "ServeOrder.h"

ServeOrder::ServeOrder(Order* o, Cook* c)
{
	order = o;
	cook = c;
	isFinished = false;
}

bool ServeOrder::isServiced()
{
	return isFinished;
}

void ServeOrder::Serve()
{
	int dishes = order->Getsize();
	int speed = cook->GetSpeed();

	//continue 
}