#include <cstdlib>
#include <time.h>
#include <iostream>
#include <string>
#include <fstream>
#include "Restaurant.h"
#include "../PromotionEvent.h"
#include "../Events/ArrivalEvent.h"
#include "../CancellationEvent.h"


Restaurant::Restaurant()
{
	pGUI = NULL;
	NumAutoPromoted = 0;
	NumNormalOrders = NumVeganOrders = NumVipOrders = 0;
	NumUrgentOrders = NumInjuredCooks = 0;
}

void Restaurant::RunSimulation()
{
	pGUI = new GUI;
	PROG_MODE mode = pGUI->getGUIMode();
	ReadFile();
	Run(mode);
	PrintFile();
}

void Restaurant::Run(PROG_MODE mode)
{
	int currentTimeStep = 0;
	while (!InServiceOrders.isEmpty() || !NormalOrders.isEmpty()
		|| !VipOrders.isEmpty() || !VeganOrders.isEmpty() || !EventsQueue.isEmpty())
	{
		currentTimeStep++;
		ExecuteEvents(currentTimeStep);
		RemoveFromBreakList(currentTimeStep);
		RemoveFromRestList(currentTimeStep);
		CompleteOrders(currentTimeStep);
		ManageOrders(currentTimeStep);
		AssignOrders(currentTimeStep);
		CheckInjuries(currentTimeStep);

		switch (mode)
		{
		case MODE_INTR:
			FillDrawingList(currentTimeStep);
			pGUI->waitForClick();
			break;

		case MODE_STEP:
			FillDrawingList(currentTimeStep);
			Sleep(1000);
			break;

		case MODE_SLNT:
			break;

		}
	}
	pGUI->PrintMessage("Simulation ended, click to continue", 1);
	pGUI->waitForClick();
}

void Restaurant::ExecuteEvents(int currentTimeStep)
{
	Event* pE;

	while (EventsQueue.peekFront(pE))	//as long as there are more events
	{
		if (pE->getEventTime() > currentTimeStep)	//no more events at current timestep
		{
			return;
		}
		pE->Execute(this);
		EventsQueue.dequeue(pE);	//remove event from the queue

		delete pE;		//deallocate event object from memory

	}
}

void Restaurant::RemoveFromBreakList(int currentTimeStep)
{
	Cook* pCook;

	while (VipInBreak.peekFront(pCook))
	{
		if (pCook->GetStartBreakTime() + pCook->GetBreakDuration() == currentTimeStep)
		{
			VipInBreak.dequeue(pCook);
			pCook->EndBreak(this);
		}
		else
		{
			break;
		}
	}

	while (VeganInBreak.peekFront(pCook))
	{
		if (pCook->GetStartBreakTime() + pCook->GetBreakDuration() == currentTimeStep)
		{
			VeganInBreak.dequeue(pCook);
			pCook->EndBreak(this);
		}
		else
		{
			break;
		}
	}
	while (NormalInBreak.peekFront(pCook))
	{
		if (pCook->GetStartBreakTime() + pCook->GetBreakDuration() == currentTimeStep)
		{
			NormalInBreak.dequeue(pCook);
			pCook->EndBreak(this);
		}
		else
		{
			break;
		}
	}
}

void Restaurant::RemoveFromRestList(int currentTimeStep)
{
	Cook* pCook;
	while (VipInRest.peekFront(pCook))
	{
		if (pCook->GetStartBreakTime() + Cook::GetRestPeriod() == currentTimeStep)
		{
			VipInRest.dequeue(pCook);
			pCook->EndRest(this);
		}
		else
		{
			break;
		}
	}

	while (VeganInRest.peekFront(pCook))
	{
		if (pCook->GetStartBreakTime() + Cook::GetRestPeriod() == currentTimeStep)
		{
			VeganInRest.dequeue(pCook);
			pCook->EndRest(this);
		}
		else
		{
			break;
		}
	}

	while (NormalInRest.peekFront(pCook))
	{
		if (pCook->GetStartBreakTime() + Cook::GetRestPeriod() == currentTimeStep)
		{
			NormalInRest.dequeue(pCook);
			pCook->EndRest(this);
		}
		else
		{
			break;
		}
	}
}

void Restaurant::CompleteOrders(int currentTimeStep)
{
	OrderService* pServe = nullptr;

	while (InServiceOrders.GetEntry(pServe, currentTimeStep))
	{
		InServiceOrders.Delete(pServe);
		pServe->FinishOrder(this, currentTimeStep);
	}
}

void Restaurant::ManageOrders(int currentTimeStep)
{
	NormalOrder* pNorm;

	while (NormalOrders.peekHead(pNorm))
	{
		if (pNorm->GetAT() + NormalOrder::GetAutoPromote() == currentTimeStep)
		{
			PromoteOrder(pNorm->GetID(), currentTimeStep);
			NumAutoPromoted++;
		}
		else
		{
			break;
		}
	}

	VipOrder* pVip;
	while (VipOrders.GetEntry(pVip, currentTimeStep)) //getting all the urgent orders in the current time step
	{
		NumUrgentOrders++;
		pVip->Urgent();
		UrgentOrders.enqueue(pVip);
		VipOrders.Delete(pVip);
	}

	while (UrgentOrders.peekFront(pVip)) //serving all the urgent orders
	{
		Cook* pCook = FindCookForUrgentOrder(pVip);
		if (pCook)
		{
			UrgentOrders.dequeue(pVip);
			OrderService* pServe = new OrderService(pVip, pCook, currentTimeStep);
			pServe->Serve(this);
		}
		else
		{
			break;
		}
	}
}

void Restaurant::AssignOrders(int currentTimeStep)
{
	OrderService* pServe = 0;
	Cook* pCook;

	VipOrder* pVip;
	while (VipOrders.peekHead(pVip))
	{
		pCook = FindCook(pVip);
		if (pCook)
		{
			pServe = new OrderService(pVip, pCook, currentTimeStep);
			pServe->Serve(this);
			VipOrders.Delete(pVip);
		}
		else
		{
			break;
		}
	}

	VeganOrder* pVegan;
	while (VeganOrders.peekFront(pVegan))
	{
		pCook = FindCook(pVegan);
		if (pCook)
		{
			pServe = new OrderService(pVegan, pCook, currentTimeStep);
			pServe->Serve(this);
			VeganOrders.dequeue(pVegan);
		}
		else
		{
			break;
		}
	}

	NormalOrder* pNorm;
	while (NormalOrders.peekHead(pNorm))
	{
		pCook = FindCook(pNorm);
		if (pCook)
		{
			pServe = new OrderService(pNorm, pCook, currentTimeStep);
			pServe->Serve(this);
			NormalOrders.Delete(pNorm);
		}
		else
		{
			break;
		}
	}
}

Restaurant::~Restaurant()
{
	if (pGUI)
	{
		delete pGUI;
		pGUI = nullptr;
	}
}


void Restaurant::FillDrawingList(int currentTimeStep)
{
	int normnum, vegannum, vipnum;
	int normcnum, vegancnum, vipcnum;
	string str;
	int servedSoFarN = 0, servedSoFarVG = 0, servedSoFarV = 0;

	pGUI->ResetDrawingList();

	string timestep;

	timestep += to_string(currentTimeStep);

	int cnt = FinishedOrders.GetLength();
	Order** arr = FinishedOrders.toArray();
	for (int i = 0; i < cnt; i++)
	{
		pGUI->AddToDrawingList(arr[i]);
		switch (arr[i]->GetType())
		{
		case TYPE_NRM:
			servedSoFarN++;
			break;
		case TYPE_VGAN:
			servedSoFarVG++;
			break;
		case TYPE_VIP:
			servedSoFarV++;
			break;
		}
	}

	cnt = InServiceOrders.GetLength();
	OrderService** arr2 = InServiceOrders.toArray();
	for (int i = 0; i < cnt; i++)
	{
		Order* pOrder = arr2[i]->GetOrder();
		Cook* pCook = arr2[i]->GetCook();
		pGUI->AddToDrawingList(pOrder);
		switch (pOrder->GetType())
		{
		case TYPE_NRM:
			servedSoFarN++;
			break;
		case TYPE_VGAN:
			servedSoFarVG++;
			break;
		case TYPE_VIP:
			servedSoFarV++;
			break;
		}
		if (arr2[i]->GetStartTime() == currentTimeStep)
		{
			switch (pCook->GetType())
			{
			case TYPE_NRM:
				str += "N";
				break;
			case TYPE_VGAN:
				str += "VG";
				break;
			case TYPE_VIP:
				str += "V";
				break;
			}
			str += to_string(pCook->GetID()) + "(";
			switch (pOrder->GetType())
			{
			case TYPE_NRM:
				str += "N";
				break;
			case TYPE_VGAN:
				str += "VG";
				break;
			case TYPE_VIP:
				str += "V";
				break;
			}
			str += to_string(pOrder->GetID()) + ") ";
		}

	}

	NormalOrder** pNorm;
	normnum = cnt = NormalOrders.GetLength();
	pNorm = NormalOrders.toArray();
	for (int i = 0; i < cnt; i++)
	{
		pGUI->AddToDrawingList(pNorm[i]);
	}

	VeganOrder** pVgn;
	pVgn = VeganOrders.toArray(cnt);
	vegannum = cnt;
	for (int i = 0; i < cnt; i++)
	{
		pGUI->AddToDrawingList(pVgn[i]);
	}

	VipOrder** pVip;
	pVip = VipOrders.toArray();
	cnt = VipOrders.GetLength();
	vipnum = cnt;
	for (int i = 0; i < cnt; i++)
	{
		pGUI->AddToDrawingList(pVip[i]);
	}



	Cook** co = NormalCooks.toArray(cnt);
	normcnum = cnt;
	for (int i = 0; i < cnt; i++)
	{
		pGUI->AddToDrawingList(co[i]);
	}

	co = VeganCooks.toArray(cnt);
	vegancnum = cnt;
	for (int i = 0; i < cnt; i++)
	{
		pGUI->AddToDrawingList(co[i]);
	}

	co = VipCooks.toArray(cnt);
	vipcnum = cnt;
	for (int i = 0; i < cnt; i++)
	{
		pGUI->AddToDrawingList(co[i]);
	}


	pGUI->PrintMessage("TS: " + timestep + "   NRMWOrders: " + to_string(normnum) +
		"	 VGNWOrders: " + to_string(vegannum) + "   VIPWOrders: " + to_string(vipnum) +
		"   NRMCooks: " + to_string(normcnum) + "   VGNCooks: " + to_string(vegancnum) +
		"   VIPCooks: " + to_string(vipcnum), 1);

	pGUI->PrintMessage(str, 2);

	pGUI->PrintMessage("servedNRM: " + to_string(servedSoFarN) + " servedVGAN: " + to_string(servedSoFarVG)
		+ " servedVIP: " + to_string(servedSoFarV), 3);


	pGUI->UpdateInterface();
}


void Restaurant::AddToInserviceList(OrderService* pServe)
{
	InServiceOrders.InsertEnd(pServe);
}

void Restaurant::AddToFinishedList(Order* ord)
{
	FinishedOrders.Insert(ord);
}


void Restaurant::AddToBreakList(Cook* pCook)
{
	if (pCook->GetType() == TYPE_VIP)
	{
		VipInBreak.enqueue(pCook);
		return;
	}
	if (pCook->GetType() == TYPE_NRM)
	{
		NormalInBreak.enqueue(pCook);
		return;
	}
	VeganInBreak.enqueue(pCook);
}

void Restaurant::AddToRestList(Cook* pCook)
{
	if (pCook->GetType() == TYPE_VIP)
	{
		VipInRest.enqueue(pCook);
		return;
	}

	if (pCook->GetType() == TYPE_VGAN)
	{
		VeganInRest.enqueue(pCook);
		return;
	}

	if (pCook->GetType() == TYPE_NRM)
	{
		NormalInRest.enqueue(pCook);
		return;
	}
}


void Restaurant::AddToCookList(Cook* pCook)
{
	if (pCook->GetType() == TYPE_VIP)
	{
		VipCooks.enqueue(pCook);
		return;
	}
	if (pCook->GetType() == TYPE_NRM)
	{
		NormalCooks.enqueue(pCook);
		return;
	}
	VeganCooks.enqueue(pCook);
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
	VipOrders.Insert(po);
	NumVipOrders++;
}

Cook* Restaurant::FindCook(Order* ord)
{
	Cook* AppropiateCook;

	if (dynamic_cast <VipOrder*> (ord))
	{
		if (VipCooks.dequeue(AppropiateCook))
		{
			return AppropiateCook;
		}
		else if (NormalCooks.dequeue(AppropiateCook))
		{
			return AppropiateCook;
		}
		else if (VeganCooks.dequeue(AppropiateCook))
		{
			return AppropiateCook;
		}
		else
		{
			return nullptr;
		}
	}

	if (dynamic_cast <NormalOrder*> (ord))
	{
		if (NormalCooks.dequeue(AppropiateCook))
		{
			return AppropiateCook;
		}
		else if (VipCooks.dequeue(AppropiateCook))
		{
			return AppropiateCook;
		}
		else
		{
			return nullptr;
		}
	}

	if (dynamic_cast <VeganOrder*> (ord))
	{
		if (VeganCooks.dequeue(AppropiateCook))
		{
			return AppropiateCook;
		}
		else
		{
			return nullptr;
		}
	}
}

Cook* Restaurant::FindCookForUrgentOrder(VipOrder* pOrd)
{
	Cook* pCook = FindCook(pOrd);

	if (pCook)
	{
		return pCook;
	}

	if (VipInBreak.dequeue(pCook))
	{
		return pCook;
	}

	if (NormalInBreak.dequeue(pCook))
	{
		return pCook;
	}

	if (VeganInBreak.dequeue(pCook))
	{
		return pCook;
	}

	if (VipInRest.dequeue(pCook))
	{
		return pCook;
	}

	if (NormalInRest.dequeue(pCook))
	{
		return pCook;
	}

	if (VeganInRest.dequeue(pCook))
	{
		return pCook;
	}

	return nullptr;
}

void Restaurant::CheckInjuries(int currentTimeStep)
{
	srand((int)time(NULL));
	float R = (rand() % 101) / 100.0;

	if (R <= InjProb)
	{
		OrderService* pServe;
		if (InServiceOrders.GetEntry(pServe))
		{
			pServe->InjureCook(currentTimeStep);
			NumInjuredCooks++;
		}
	}
}

NormalOrder* Restaurant::GetNormalOrderFromID(int ID)
{
	NormalOrder* ord = nullptr;
	NormalOrders.GetEntry(ord, ID);
	return ord;
}

void Restaurant::CancelOrder(int ID)
{
	NormalOrder* pOrd = GetNormalOrderFromID(ID);
	if (NormalOrders.Delete(pOrd))
	{
		NumNormalOrders--;
	}
}


void Restaurant::PromoteOrder(int ID, int time)
{
	NormalOrder* pOrd = GetNormalOrderFromID(ID);
	if (pOrd)
	{
		VipOrder* order = new VipOrder(pOrd, time);
		NormalOrders.Delete(pOrd);
		VipOrders.Insert(order);
		NumNormalOrders--;
		NumVipOrders++;
	}
}


void Restaurant::ReadFile()
{
	ifstream inputFile;
	string fileName = "../../Input Files/";
	string temp;
	int NormalSpeed[2], VeganSpeed[2], VipSpeed[2], maxNumOrders, NormalBreak[2], VeganBreak[2], VipBreak[2];
	int EventsNumber, AutoPromote;
	int RstPrd, Vip_WT;
	pGUI->PrintMessage("Enter the name of the input file : ", 1);
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

	Cook::SetMaxNumOrders(maxNumOrders);
	Cook::SetRestPeriod(RstPrd);
	VipOrder::SetVipWT(Vip_WT);
	NormalOrder::SetAutoPromote(AutoPromote);


	srand((int)time(NULL));
	for (int i = 0; i < NumNormalCooks; i++)
	{
		int speed = rand() % (NormalSpeed[1] - NormalSpeed[0] + 1) + NormalSpeed[0];
		int Break = rand() % (NormalBreak[1] - NormalBreak[0] + 1) + NormalBreak[0];
		Cook* pCook = new Cook(i + 1, TYPE_NRM, speed, Break);
		NormalCooks.enqueue(pCook);
	}

	for (int i = 0; i < NumVeganCooks; i++)
	{
		int speed = rand() % (VeganSpeed[1] - VeganSpeed[0] + 1) + VeganSpeed[0];
		int Break = rand() % (VeganBreak[1] - VeganBreak[0] + 1) + VeganBreak[0];
		Cook* pCook = new Cook(i + 1 + NumNormalCooks, TYPE_VGAN, speed, Break);
		VeganCooks.enqueue(pCook);
	}

	for (int i = 0; i < NumVipCooks; i++)
	{
		int speed = rand() % (VipSpeed[1] - VipSpeed[0] + 1) + VipSpeed[0];
		int Break = rand() % (VipBreak[1] - VipBreak[0] + 1) + VipBreak[0];
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

	pGUI->PrintMessage("Enter the name of the output file : ", 1);
	temp = pGUI->GetString();
	temp += ".txt";
	fileName += temp;
	OutputFile.open(fileName, ios::out);

	OutputFile << "FT\tID\tAT\tWT\tST\n";
	float WT_Sum = 0;
	float ST_Sum = 0;

	while (!FinishedOrders.isEmpty())
	{
		Order* pOrd;
		FinishedOrders.peekHead(pOrd);
		FinishedOrders.Delete(pOrd);
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
	OutputFile << "Avg Wait = " << WT_Sum / (NumNormalOrders + NumVeganOrders + NumVipOrders) << ",  Avg Serv = " << ST_Sum / (NumNormalOrders + NumVeganOrders + NumVipOrders) << "\n";
	OutputFile << "Urgent orders: " << NumUrgentOrders << ",   ";
	OutputFile << "Auto - promoted: ";
	NumNormalOrders == 0 && NumAutoPromoted == 0 ? OutputFile << 0 : OutputFile << (float(NumAutoPromoted) / (NumNormalOrders + NumAutoPromoted)) * 100;
	OutputFile << " %";
	OutputFile.close();
}