#include"Events/Event.h"

class promote : public Event
{
	double money;
public:

	promote(int eTime, int oID,int r_money);

	virtual void Execute(Restaurant* pRest);	//overrided execute function



};
