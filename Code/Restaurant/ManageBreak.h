#pragma once

#include "Rest/Cook.h"
#include "Rest/Restaurant.h"

class ManageBreak
{
	Cook* pCook;

public:

	ManageBreak(Cook* cook);

	void EndBreak(Restaurant* pRest);

	void EndRest(Restaurant* pRest);
};

