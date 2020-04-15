#include <cstdlib>
#include <time.h>
#include <iostream>
#include <string>
#include <fstream>

#include"..\PromotionEvent.h"

#include "Restaurant.h"

#include "..\Events\ArrivalEvent.h"

#include"..\CancellationEvent.h"




Restaurant::Restaurant()

{
	pGUI = NULL;
}



void Restaurant::RunSimulation()
{
	pGUI = new GUI;

	PROG_MODE	mode = pGUI->getGUIMode();



	switch (mode)	
	{

	case MODE_INTR:
		simulate();
		break;

	case MODE_STEP:

		break;

	case MODE_SLNT:

		break;

	};
}







//////////////////////////////////  Event handling functions   /////////////////////////////




void Restaurant::ExecuteEvents(int CurrentTimeStep)
{
	Event* pE;

	while (EventsQueue.peekFront(pE))	//as long as there are more events
	{

		if (pE->getEventTime() > CurrentTimeStep)	//no more events at current timestep

			return;



		pE->Execute(this);

		EventsQueue.dequeue(pE);	//remove event from the queue

		delete pE;		//deallocate event object from memory

	}
}

Restaurant::~Restaurant()

{

	if (pGUI)

		delete pGUI;

}

void Restaurant::simulate()
{
	int timestep = 0;
	int ordernum = 0;
	ReadFile();
	pGUI->PrintMessage("Click to continue");
	while (!InServiceOrders.isempty() || !NormalOrders.isempty()
		|| !VipOrders.isEmpty() || !VeganOrders.isEmpty() ||
		!EventsQueue.isEmpty())
	{
		pGUI->waitForClick();
		timestep++;
		ExecuteEvents(timestep);
		Order* ptr;
		if (NormalOrders.dequeue(ptr))
		{
			ptr->setStatus(SRV);
			InServiceOrders.InsertEnd(ptr);
		}
		if (VipOrders.dequeue(ptr))
		{
			ptr->setStatus(SRV);
			InServiceOrders.InsertEnd(ptr);
		}
		if (VeganOrders.dequeue(ptr))
		{
			ptr->setStatus(SRV);
			InServiceOrders.InsertEnd(ptr);
		}
		if (timestep % 5 == 0)
		{
			int t = 3;
			while (t--)
			{
				if (!InServiceOrders.dequeue(ptr))
					break;
				ptr->setStatus(DONE);
				FinishedOrders.enqueue(ptr);
			}
		}
		FillDrawingList(timestep);
	}
	pGUI->PrintMessage("Simulation ended, click to exit");
	pGUI->waitForClick();
}

void Restaurant::FillDrawingList(int CurrentTimeStep)
{
	pGUI->ResetDrawingList();

	string timestep;

	timestep += to_string(CurrentTimeStep);

	int orders;
	Order** ord = FinishedOrders.toArray(orders);
	for (int i = 0; i < orders; i++)
	{
		pGUI->AddToDrawingList(ord[i]);
	}

	ord = InServiceOrders.toArray(orders);
	int inser = orders;
	for (int i = 0; i < orders; i++)
	{
		pGUI->AddToDrawingList(ord[i]);
	}

	ord = NormalOrders.toArray(orders);
	int normnum = orders;
	for (int i = 0; i < orders; i++)
	{
		pGUI->AddToDrawingList(ord[i]);
	}

	ord = VeganOrders.toArray(orders);
	int vegannum = orders;
	for (int i = 0; i < orders; i++)
	{
		pGUI->AddToDrawingList(ord[i]);
	}

	ord = VipOrders.toArray(orders);
	int vipnum = orders;
	for (int i = 0; i < orders; i++)
	{
		pGUI->AddToDrawingList(ord[i]);
	}

	int cooks;
	Cook** co = NormalCooks.toArray(cooks);
	int normcnum = cooks - inser / 3;
	for (int i = 0; i < normcnum; i++)
	{
		pGUI->AddToDrawingList(co[i]);
	}

	co = VeganCooks.toArray(cooks);
	int vegancnum = cooks - (inser - (inser / 3)) / 2;
	for (int i = 0; i < vegancnum; i++)
	{
		pGUI->AddToDrawingList(co[i]);
	}

	co = VipCooks.toArray(cooks);
	int vipcnum = cooks - inser - (inser - (inser / 3)) / 2;
	for (int i = 0; i < vipcnum; i++)
	{
		pGUI->AddToDrawingList(co[i]);
	}

	if (vegancnum < 0)vegancnum = 0;
	if (vipcnum < 0)vipcnum = 0;
	if (normcnum < 0)normcnum = 0;

	pGUI->PrintMessage("TS : " + timestep + "   NRMWOrders: " + to_string(normnum) +
		"	 VGNWOrders: " + to_string(vegannum) + "   VIPWOrders: " + to_string(vipnum) + "   NRMCooks: " + to_string(normcnum) +
		"   VGNCooks: " + to_string(vegancnum) + "   VIPCooks: " + to_string(vipcnum));


	pGUI->UpdateInterface();

}


void Restaurant::addtonormarlist(Order* po)
{
	NormalOrders.InsertEnd(po);
}
void Restaurant::addtoveganlist(Order* po)
{
	VeganOrders.enqueue(po);
}
void Restaurant::addtoviplist(Order* po)
{
	VipOrders.enqueue(po);
}

void Restaurant::cancelorder(int r_ID)
{
	NormalOrders.deletebyid(r_ID);
}

void Restaurant::PromoteOrder(int Oid)
{}


int Restaurant::GetNumNormal()
{
	return NumNormalCooks;
}

int Restaurant::GetNumVegan()
{
	return NumVeganCooks;
}

int Restaurant::GetNumVip()
{
	return NumVipCooks;
}

void Restaurant::ReadFile()
{
	ifstream inputFile;

	string fileName = "../../Input Files/";

	string temp;

	pGUI->PrintMessage("Enter the name of the file : ");

	temp = pGUI->GetString();

	temp += ".txt";

	fileName += temp;

	inputFile.open(fileName, ios::in);


	inputFile >> NumNormalCooks >> NumVeganCooks >> NumVipCooks;

	inputFile >> NormalSpeed >> VeganSpeed >> VipSpeed;

	inputFile >> maxCooks >> NormalBreak >> VeganBreak >> VipBreak;

	inputFile >> AutoPromoted;

	inputFile >> EventsNumber;

	for (int i = 0; i < NumNormalCooks; i++)
	{
		Cook* t = new Cook(i + 1, TYPE_NRM, NormalSpeed);
		NormalCooks.enqueue(t);
	}

	for (int i = 0; i < NumVeganCooks; i++)
	{
		Cook* t = new Cook(i + 1 + NumNormalCooks, TYPE_VGAN, VeganSpeed);
		VeganCooks.enqueue(t);
	}

	for (int i = 0; i < NumVipCooks; i++)
	{
		Cook* t = new Cook(i + 1 + NumNormalCooks + NumVeganCooks, TYPE_VIP, VipSpeed);
		VipCooks.enqueue(t);
	}


	//Reading Events

	for (int i = 0; i < EventsNumber; i++)

	{

		int ID;   //id of the order

		int TS;  //timestep 



		Event* pEvent = NULL;

		char EventChar;

		inputFile >> EventChar;

		switch (EventChar)
		{

		case 'R':

			char orderType;

			int size;

			double orderMoney;

			inputFile >> orderType >> TS >> ID >> size >> orderMoney;

			switch (orderType)
			{

			case 'N':

				pEvent = new ArrivalEvent(TS, ID, TYPE_NRM, orderMoney, size);
				break;

			case 'G':

				pEvent = new ArrivalEvent(TS, ID, TYPE_VGAN, orderMoney, size);
				break;

			case 'V':

				pEvent = new ArrivalEvent(TS, ID, TYPE_VIP, orderMoney, size);
				break;

			}

			EventsQueue.enqueue(pEvent);

			break;



		case 'P':
			double ExtraMoney;
			inputFile >> TS >> ID >> ExtraMoney;
			pEvent = new PromotionEvent(TS, ID, ExtraMoney);
			EventsQueue.enqueue(pEvent);
			break;

		case 'X':
			inputFile >> TS >> ID;
			pEvent = new CancellationEvent(TS, ID);
			EventsQueue.enqueue(pEvent);
			break;
		}
	}

}
