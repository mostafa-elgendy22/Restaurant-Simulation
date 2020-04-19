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
	pServ = NULL;
	currentTimeStep = 0;
}


void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE mode = pGUI->getGUIMode();

	switch (mode)
	{
	case MODE_INTR:
		ReadFile();
		RunInteractive();
		break;

	case MODE_STEP:
		ReadFile();
		RunStepByStep();
		break;

	case MODE_SLNT:
		ReadFile();
		PrintFile();
		break;
	};
}


void Restaurant::RunInteractive()
{

}


void Restaurant::RunStepByStep()
{

}

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

void Restaurant::AssignOrders()
{

}

Restaurant::~Restaurant()
{
	if (pGUI)
	{
		delete pGUI;
	}
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

	NormalOrder** pNorm;
	pNorm = NormalOrders.toArray(orders);
	int normnum = orders;
	for (int i = 0; i < orders; i++)
	{
		pGUI->AddToDrawingList(pNorm[i]);
	}

	VeganOrder** pVgn;
	pVgn = VeganOrders.toArray(orders);
	int vegannum = orders;
	for (int i = 0; i < orders; i++)
	{
		pGUI->AddToDrawingList(pVgn[i]);
	}

	VipOrder** pVip;
	pVip = VipOrders.toArray(orders);
	int vipnum = orders;
	for (int i = 0; i < orders; i++)
	{
		pGUI->AddToDrawingList(pVip[i]);
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


void Restaurant::AddToNormalList(NormalOrder* po)
{
	NormalOrders.InsertEnd(po);
}
void Restaurant::AddToVeganList(VeganOrder* po)
{
	VeganOrders.enqueue(po);
}
void Restaurant::AddToVipList(VipOrder* po)
{
	VipOrders.enqueue(po);
}

void Restaurant::CancelOrder(int r_ID)
{
	NormalOrders.DeleteByID(r_ID);
}

void Restaurant::PromoteOrder(int Oid)
{

}

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
	int AutoPromote, NormalSpeed, VeganSpeed, VipSpeed, maxNumOrders, NormalBreak, VeganBreak, VipBreak;

	pGUI->PrintMessage("Enter the name of the input file : ");
	temp = pGUI->GetString();
	temp += ".txt";
	fileName += temp;
	inputFile.open(fileName, ios::in);


	inputFile >> NumNormalCooks >> NumVeganCooks >> NumVipCooks;
	inputFile >> NormalSpeed >> VeganSpeed >> VipSpeed;
	inputFile >> maxNumOrders >> NormalBreak >> VeganBreak >> VipBreak;
	inputFile >> AutoPromote;
	inputFile >> EventsNumber;

	for (int i = 0; i < NumNormalCooks; i++)
	{
		Cook* c = new Cook(i + 1, TYPE_NRM, NormalSpeed, maxNumOrders, NormalBreak);
		NormalCooks.enqueue(c);
	}

	for (int i = 0; i < NumVeganCooks; i++)
	{
		Cook* c = new Cook(i + 1 + NumNormalCooks, TYPE_VGAN, VeganSpeed, maxNumOrders, VeganBreak);
		VeganCooks.enqueue(c);
	}

	for (int i = 0; i < NumVipCooks; i++)
	{
		Cook* c = new Cook(i + 1 + NumNormalCooks + NumVeganCooks, TYPE_VIP, VipSpeed, maxNumOrders, VipBreak);
		VipCooks.enqueue(c);
	}

	//Reading Events

	for (int i = 0; i < EventsNumber; i++)
	{
		int ID;      //id of the order
		int TS;      //timestep 
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

void Restaurant::PrintFile()
{
	ofstream OutputFile;
	string fileName = "../../Output Files/";
	string temp;

	pGUI->PrintMessage("Enter the name of the output file : ");
	temp = pGUI->GetString();
	temp += ".txt";
	fileName += temp;
	OutputFile.open(fileName, ios::out);

	//print info here
}
