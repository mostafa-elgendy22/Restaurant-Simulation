#include "promote.h"


promote::promote(int eTime, int oID,int r_money):Event(eTime,oID)
{
	money = r_money;
}

void promote::Execute(Restaurant* pRest)
{

}