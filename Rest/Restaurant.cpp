#include <cstdlib>

#include <time.h>

#include <iostream>

#include <string>

#include <fstream>

using namespace std;



#include "Restaurant.h"

#include "..\Events\ArrivalEvent.h"

#include"..\CancellationEvent.h"

#include"..\PromotionEvent.h"




Restaurant::Restaurant()

{

	pGUI = NULL;

}



void Restaurant::RunSimulation()
{

	pGUI = new GUI;

	PROG_MODE	mode = pGUI->getGUIMode();



	switch (mode)	//Add a function for each mode in next phases

	{

	case MODE_INTR:
		simulate();
		break;

	case MODE_STEP:

		break;

	case MODE_SLNT:

		break;

	case MODE_DEMO:
		Just_A_Demo();



	};



}







//////////////////////////////////  Event handling functions   /////////////////////////////



//Executes ALL events that should take place at current timestep

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
	while (!inservice.isempty() || !normalorders.isempty()
		|| !viporders.isEmpty() || !veganorders.isEmpty())
	{
		timestep++;
		ExecuteEvents(timestep);
		Order* ptr;
		if (normalorders.dequeue(ptr))
		{
			inservice.InsertEnd(ptr);
		}
		if (viporders.dequeue(ptr))
		{
			inservice.InsertEnd(ptr);
		}
		if (veganorders.dequeue(ptr))
		{
			inservice.InsertEnd(ptr);
		}
		if (timestep % 5 == 0)
		{
			int t = 3;
			while (t--)
			{
				if (!inservice.dequeue(ptr))
					break;
				finished.InsertEnd(ptr);
			}
		}
		FillDrawingList(timestep);
	}

}

void Restaurant::FillDrawingList(int CurrentTimeStep)

{

	//This function should be implemented in phase1

	//It should add ALL orders and Cooks to the drawing list

	//It should get orders from orders lists/queues/stacks/whatever (same for Cooks)

	//To add orders it should call function  void GUI::AddToDrawingList(Order* pOrd);

	//To add Cooks it should call function  void GUI::AddToDrawingList(Cook* pCc);

	char timestep[10];

	itoa(CurrentTimeStep, timestep, 10);

	pGUI->PrintMessage(timestep);

	//pGUI->PrintMessage(" ")

	int orders;
	Order** ord = finished.toArray(orders);

	for (int i = 0; i < orders; orders++)
	{
		pGUI->AddToDrawingList(ord[i]);
	}

	int cooks;
	Cook** co = cooklist.toArray(cooks);

	for (int i = 0; i < cooks; i++)
	{
		pGUI->AddToDrawingList(co[i]);
	}

	Sleep(100);

	pGUI->UpdateInterface();

}









//////////////////////////////////////////////////////////////////////////////////////////////

/// ==> 

///  DEMO-related functions. Should be removed in phases 1&2



//Begin of DEMO-related functions



//This is just a demo function for project introductory phase

//It should be removed starting phase 1

void Restaurant::Just_A_Demo()

{



	//

	// THIS IS JUST A DEMO FUNCTION

	// IT SHOULD BE REMOVED IN PHASE 1 AND PHASE 2



	int EventCnt;

	Order* pOrd;

	Event* pEv;

	srand(time(NULL));



	pGUI->PrintMessage("Just a Demo. Enter EVENTS Count(next phases should read I/P filename):");

	EventCnt = atoi(pGUI->GetString().c_str());	//get user input as a string then convert to integer



	pGUI->PrintMessage("Generating Events randomly... In next phases, Events should be loaded from a file...CLICK to continue");

	pGUI->waitForClick();



	//Just for sake of demo, generate some cooks and add them to the drawing list

	//In next phases, Cooks info should be loaded from input file

	int C_count = 12;

	Cook* pC = new Cook[C_count];

	int cID = 1;



	for (int i = 0; i < C_count; i++)

	{

		cID += (rand() % 15 + 1);

		pC[i].setID(cID);

		pC[i].setType((ORD_TYPE)(rand() % TYPE_CNT));

	}





	int EvTime = 0;



	int O_id = 1;



	//Create Random events and fill them into EventsQueue

	//All generated event will be "ArrivalEvents" for the demo

	for (int i = 0; i < EventCnt; i++)

	{

		O_id += (rand() % 4 + 1);

		int OType = rand() % TYPE_CNT;	//Randomize order type		

		EvTime += (rand() % 5 + 1);			//Randomize event time

		int money = (rand() % 2);

		int size = (rand() % 6);

		pEv = new ArrivalEvent(EvTime, O_id, (ORD_TYPE)OType, money, size);

		EventsQueue.enqueue(pEv);



	}



	// --->   In next phases, no random generation is done

	// --->       EventsQueue should be filled from actual events loaded from input file


	//Now We have filled EventsQueue (randomly)

	int CurrentTimeStep = 1;

	//as long as events queue is not empty yet

	while (!EventsQueue.isEmpty())
	{

		//print current timestep

		char timestep[10];

		itoa(CurrentTimeStep, timestep, 10);

		pGUI->PrintMessage(timestep);





		//The next line may add new orders to the DEMO_Queue

		ExecuteEvents(CurrentTimeStep);	//execute all events at current time step





/////////////////////////////////////////////////////////////////////////////////////////

		/// The next code section should be done through function "FillDrawingList()" once you

		/// decide the appropriate list type for Orders and Cooks



		//Let's add ALL randomly generated Cooks to GUI::DrawingList

		for (int i = 0; i < C_count; i++)

			pGUI->AddToDrawingList(&pC[i]);



		//Let's add ALL randomly generated Ordes to GUI::DrawingList

		int size = 0;

		Order** Demo_Orders_Array = DEMO_Queue.toArray(size);



		for (int i = 0; i < size; i++)

		{

			pOrd = Demo_Orders_Array[i];

			pGUI->AddToDrawingList(pOrd);

		}

		/////////////////////////////////////////////////////////////////////////////////////////



		pGUI->UpdateInterface();

		Sleep(1000);

		CurrentTimeStep++;	//advance timestep

		pGUI->ResetDrawingList();

	}



	delete[]pC;





	pGUI->PrintMessage("generation done, click to END program");

	pGUI->waitForClick();





}

////////////////



void Restaurant::AddtoDemoQueue(Order* pOrd)

{

	DEMO_Queue.enqueue(pOrd);

}



/// ==> end of DEMO-related function

//////////////////////////////////////////////////////////////////////////////////////////////

void Restaurant::addtonormarlist(Order* po)
{
	normalorders.InsertEnd(po);
}
void Restaurant::addtoveganlist(Order* po)
{
	veganorders.enqueue(po);
}
void Restaurant::addtoviplist(Order* po)
{
	viporders.enqueue(po);
}

void Restaurant::cancelorder(int r_ID)
{
	normalorders.deletebyid(r_ID);
}


int Restaurant::GetNumNormal()

{

	return NormalCooks;

}



int Restaurant::GetNumVegan()

{

	return VeganCooks;

}

int Restaurant::GetNumVip()
{

	return VipCooks;

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


	inputFile >> NormalCooks >> VeganCooks >> VipCooks;

	inputFile >> NormalSpeed >> VeganSpeed >> VipSpeed;

	inputFile >> maxCooks >> NormalBreak >> VeganBreak >> VipBreak;

	inputFile >> AutoPromoted;

	inputFile >> EventsNumber;

	for (int i = 0; i < NormalCooks; i++)
	{
		Cook* t = new Cook(i + 1, TYPE_NRM, NormalSpeed);
		cooklist.InsertEnd(t);
	}

	for (int i = 0; i < VeganCooks; i++)
	{
		Cook* t = new Cook(i + 1 + NormalCooks, TYPE_VGAN, VeganSpeed);
		cooklist.InsertEnd(t);
	}

	for (int i = 0; i < VipCooks; i++)
	{
		Cook* t = new Cook(i + 1 + NormalCooks + VeganCooks, TYPE_VIP, VipSpeed);
		cooklist.InsertEnd(t);
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