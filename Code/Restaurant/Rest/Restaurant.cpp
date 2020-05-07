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
	NumAutoPromoted = 0;
	NumNormalOrders = NumVeganOrders = NumVipOrders = 0;
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
		PrintFile();
		break;

	case MODE_STEP:
		ReadFile();
		RunStepByStep();
		PrintFile();
		break;

	case MODE_SLNT:
		ReadFile();
		//simulation function call here
		PrintFile();
		break;
	};
}


void Restaurant::RunInteractive()
{
	FillDrawingList();

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
		pGUI = nullptr;
	}
}


void Restaurant::FillDrawingList()
{
	pGUI->ResetDrawingList();

	string timestep;

	timestep += to_string(currentTimeStep);

	int orders = FinishedOrders.GetLength();
	Order** ord = FinishedOrders.toArray();
	for (int i = 0; i < orders; i++)
	{
		pGUI->AddToDrawingList(ord[i]);
	}

	ord = InServiceOrders.toArray();
	int inser = orders;
	for (int i = 0; i < orders; i++)
	{
		pGUI->AddToDrawingList(ord[i]);
	}

	NormalOrder** pNorm;
	pNorm = NormalOrders.toArray();
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
	NumNormalOrders++;
}

void Restaurant::AddToVeganList(VeganOrder* po)
{
	VeganOrders.enqueue(po);
	NumVeganOrders++;
}

void Restaurant::AddToVipList(VipOrder* po)
{
	VipOrders.enqueue(po, 6.5);
	NumVipOrders++;
}

NormalOrder*& Restaurant::GetNormalOrderFromID(int ID)
{
	NormalOrder* ord = nullptr;
	NormalOrders.GetEntry(ord, ID);
	return ord;
}

void Restaurant::CancelOrder(int ID)
{
	if (NormalOrders.Delete(GetNormalOrderFromID(ID)))
	{
		NumNormalOrders--;
	}
}


void Restaurant::PromoteOrder(int ID)
{
	NormalOrder* pOrd = GetNormalOrderFromID(ID);
	if (pOrd)
	{
		VipOrder* order = new VipOrder(pOrd);
		NormalOrders.Delete(pOrd);
		VipOrders.enqueue(order, order->GetPriorityFactor());
		NumNormalOrders--;
		NumVipOrders++;
		NumAutoPromoted++;
	}
}


void Restaurant::ReadFile()
{
	ifstream inputFile;
	string fileName = "../../Input Files/";
	string temp;
	int AutoPromote, NormalSpeed[2], VeganSpeed[2], VipSpeed[2], maxNumOrders, NormalBreak[2], VeganBreak[2], VipBreak[2];
	int EventsNumber;
	pGUI->PrintMessage("Enter the name of the input file : ");
	temp = pGUI->GetString();
	temp += ".txt";
	fileName += temp;
	inputFile.open(fileName, ios::in);


	inputFile >> NumNormalCooks >> NumVeganCooks >> NumVipCooks;
	inputFile >> NormalSpeed[0] >> NormalSpeed[1];
	inputFile >> VeganSpeed[0] >> VeganSpeed[1];
	inputFile >> VipSpeed[0] >> VipSpeed[1];
	inputFile >> maxNumOrders;
	inputFile >> NormalBreak[0] >> NormalBreak[1];
	inputFile >> VeganBreak[0] >> VeganBreak[1];
	inputFile >> VipBreak[0] >> VipBreak[1];
	inputFile >> InjProb >> RstPrd;
	inputFile >> AutoPromote >> Vip_WT;
	inputFile >> EventsNumber;


	srand((int)time(NULL));
	for (int i = 0; i < NumNormalCooks; i++)
	{
		int speed = NormalSpeed[0] + rand() % NormalSpeed[1];
		int Break = NormalBreak[0] + rand() % NormalBreak[1];
		Cook* pCook = new Cook(i + 1, TYPE_NRM, speed, Break);
		NormalCooks.enqueue(pCook);
	}

	for (int i = 0; i < NumVeganCooks; i++)
	{
		int speed = VeganSpeed[0] + rand() % VeganSpeed[1];
		int Break = VeganBreak[0] + rand() % VeganBreak[1];
		Cook* pCook = new Cook(i + 1 + NumNormalCooks, TYPE_VGAN, speed, Break);
		VeganCooks.enqueue(pCook);
	}

	for (int i = 0; i < NumVipCooks; i++)
	{
		int speed = VipSpeed[0] + rand() % VipSpeed[1];
		int Break = VipBreak[0] + rand() % VipBreak[1];
		Cook* pCook = new Cook(i + 1 + NumNormalCooks + NumVeganCooks, TYPE_VIP, speed, Break);
		VipCooks.enqueue(pCook);
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

	OutputFile << "FT\tID\tAT\tWT\tST\n";
	FinishedOrders.Sort();
	float WT_Sum = 0;
	float ST_Sum = 0;

	for (int i = 0; i < FinishedOrders.GetLength(); i++)
	{
		Order* pOrd;
		FinishedOrders.head(pOrd);
		int WT = pOrd->GetFT() - pOrd->GetAT() - pOrd->GetST();
		OutputFile << pOrd->GetFT() << "\t" << pOrd->GetID() << "\t" << pOrd->GetAT() << "\t";
		OutputFile << WT << "\t" << pOrd->GetST() << "\n";
		WT_Sum += WT;
		ST_Sum += pOrd->GetST();
	}

	OutputFile << "Orders: " << NumNormalOrders + NumVeganOrders + NumVipOrders;
	OutputFile << " [Norm:" << NumNormalOrders << ", Veg:" << NumVeganOrders << ", VIP:" << NumVipOrders << "]" << "\n";
	OutputFile << "Cooks: " << NumNormalCooks + NumVipCooks + NumVeganCooks;
	OutputFile << " [Norm:" << NumNormalCooks << ", Veg:" << NumVeganCooks;
	OutputFile << ", VIP:" << NumVipCooks << ", injured: " << NumInjuredCooks << "]" << "\n";
	OutputFile << "Avg Wait = " << "" << ",  Avg Serv = " << "" << "\n";
	OutputFile << "Urgent orders: " << NumUrgentOrders << ",   ";
	OutputFile << "Auto - promoted: " << (float(NumAutoPromoted) / NumNormalOrders) * 100 << " %";
	OutputFile.close();
}

