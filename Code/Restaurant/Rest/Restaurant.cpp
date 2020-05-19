#include <cstdlib>
#include <time.h>
#include <iostream>
#include <string>
#include <fstream>
#include "Restaurant.h"
#include "../PromotionEvent.h"
#include "../Events/ArrivalEvent.h"
#include "../CancellationEvent.h"
#include "../ManageBreak.h"



Restaurant::Restaurant()
{
	pGUI = NULL;
	NumAutoPromoted = 0;
	NumNormalOrders = NumVeganOrders = NumVipOrders = 0;
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
	while (!InService_Orders_And_Cooks.isEmpty() || !NormalOrders.isEmpty()
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
	pGUI->PrintMessage("Simulation ended, click to exit",1);
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
	ManageBreak* pManage;

	while (VipInBreak.Delete(pCook, currentTimeStep))
	{
		pManage = new ManageBreak(pCook);
		pManage->EndBreak(this);
	}

	while (VeganInBreak.Delete(pCook, currentTimeStep))
	{
		pManage = new ManageBreak(pCook);
		pManage->EndBreak(this);
	}

	while (NormalInBreak.Delete(pCook, currentTimeStep))
	{
		pManage = new ManageBreak(pCook);
		pManage->EndBreak(this);
	}

}

void Restaurant::RemoveFromRestList(int currentTimeStep)
{
	Cook* pCook;
	ManageBreak* pManage;
	while (VipInRest.peekFront(pCook))
	{
		if (pCook->GetStartBreakTime() + RstPrd == currentTimeStep)
		{
			VipInRest.dequeue(pCook);
			pManage = new ManageBreak(pCook);
			pManage->EndRest(this);
		}
		else
		{
			break;
		}
	}

	while (VeganInRest.peekFront(pCook))
	{
		if (pCook->GetStartBreakTime() + RstPrd == currentTimeStep)
		{
			VeganInRest.dequeue(pCook);
			pManage = new ManageBreak(pCook);
			pManage->EndRest(this);
		}
		else
		{
			break;
		}
	}

	while (NormalInRest.peekFront(pCook))
	{
		if (pCook->GetStartBreakTime() + RstPrd == currentTimeStep)
		{
			NormalInRest.dequeue(pCook);
			pManage = new ManageBreak(pCook);
			pManage->EndRest(this);
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

	while (InService_Orders_And_Cooks.Delete(pServe, currentTimeStep))
	{
		pServe->FinishOrder(this, currentTimeStep);
	}

}

void Restaurant::ManageOrders(int currentTimeStep)
{
	NormalOrder* pNorm;

	if (NormalOrders.peekHead(pNorm))
	{
		if (pNorm->GetAT() + currentTimeStep == AutoPromote)
		{
			PromoteOrder(pNorm->GetID());
			NumAutoPromoted++;
		}
	}

	Queue<VipOrder*> tempq;
	VipOrder* temporder;
	Cook* pCook;
	OrderService* pServe;
	while (VipOrders.dequeue(temporder))
	{
		if (currentTimeStep - temporder->GetAT() < Vip_WT)
		{
			tempq.enqueue(temporder);
			continue;
		}
		pCook = FindCookForUrgentOrder(temporder);
		if (pCook)
		{
			pServe = new OrderService(temporder, pCook, currentTimeStep);
			pServe->Serve(this);
			continue;
		}
		VipOrders.enqueue(temporder);
		break;
	}
	while (tempq.dequeue(temporder))
		VipOrders.enqueue(temporder);
}

void Restaurant::AssignOrders(int currentTimeStep)
{
	OrderService* pServe = 0;
	VipOrder* pVip;
	NormalOrder* pNorm;
	VeganOrder* pVegan;
	Cook* pCook;

	while (VipOrders.peekFront(pVip))
	{
		if (pVip->GetAT() >= currentTimeStep)
		{
			pCook = FindCook(pVip);
			if (pCook)
			{
				pServe = new OrderService(pVip, pCook,currentTimeStep);
				pServe->Serve(this);
				VipOrders.dequeue(pVip);
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	while (VeganOrders.peekFront(pVegan))
	{
		if (pVegan->GetAT() >= currentTimeStep)
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
		else
		{
			break;
		}
	}

	while (NormalOrders.peekHead(pNorm))
	{
		if (pNorm->GetAT() >= currentTimeStep)
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
	int servedSoFarN=0, servedSoFarVG=0, servedSoFarV=0;

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

	
	OrderService** arr2 = InService_Orders_And_Cooks.toArray(cnt);
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
		if (arr2[i]->GetStartTime()== currentTimeStep)
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
	pVip = VipOrders.toArray(cnt);
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
	InService_Orders_And_Cooks.Insert(pServe);
}

void Restaurant::AddToFinishedList(Order* ord)
{
	FinishedOrders.InsertEnd(ord);
}

int Restaurant::GetMaxNumberOrders()
{
	return maxNumCooks;
}

void Restaurant::AddToBreakList(Cook* pCook)
{
	if (pCook->GetType() == TYPE_VIP)
	{
		VipInBreak.Insert(pCook);
		return;
	}
	if (pCook->GetType() == TYPE_NRM)
	{
		NormalInBreak.Insert(pCook);
		return;
	}
	VeganInBreak.Insert(pCook);
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
	VipOrders.enqueue(po);
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

	if (VipInBreak.Delete(pCook))
	{
		return pCook;
	}

	if (NormalInBreak.Delete(pCook))
	{
		return pCook;
	}

	if (VeganInBreak.Delete(pCook))
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
}

void Restaurant::CheckInjuries(int currentTimeStep)
{
	srand((int)time(NULL));
	float R = static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	if (R <= InjProb)
	{
		OrderService* pServe;
		if (InService_Orders_And_Cooks.GetEntry(pServe))
		{
			pServe->InjureCook(currentTimeStep);
		}
	}
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
		VipOrders.enqueue(order);
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
	int EventsNumber;
	pGUI->PrintMessage("Enter the name of the input file : ",1);
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

	pGUI->PrintMessage("Enter the name of the output file : ",1);
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
	OutputFile << "Avg Wait = " << "" << ",  Avg Serv = " << "" << "\n";
	OutputFile << "Urgent orders: " << NumUrgentOrders << ",   ";
	OutputFile << "Auto - promoted: " << (float(NumAutoPromoted) / NumNormalOrders) * 100 << " %";
	OutputFile.close();
}

/*void Restaurant::simulate()
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
*/