#ifndef __RESTAURANT_H_
#define __RESTAURANT_H_

#include "..\Defs.h"
#include "..\CMUgraphicsLib\CMUgraphics.h"
#include "..\GUI\GUI.h"
#include "..\Generic_DS\Queue.h"
#include "..\Events\Event.h"
#include "../PriorityQueue.h"
#include "../LinkedList.h"
#include "Order.h"
#include "../VipOrder.h"
#include "../VeganOrder.h"
#include "../NormalOrder.h"
#include "../ServeOrder.h"


class Restaurant
{
private:

	GUI* pGUI;
	Queue<Event*> EventsQueue;	   //Queue of all events that will be loaded from file
	ServeOrder* pServ;

	PriorityQueue<VipOrder*> VipOrders;
	LinkedList<NormalOrder*> NormalOrders;
	Queue<VeganOrder*> VeganOrders;

	LinkedList<Order*>InServiceOrders;
	Queue<Order*>FinishedOrders;

	Queue<Cook*>NormalCooks;
	Queue<Cook*>VeganCooks;
	Queue<Cook*>VipCooks;

	Queue<Cook*>NormalInBreak;
	Queue<Cook*>VeganInBreak;
	Queue<Cook*>VipInBreak;

	int NumNormalCooks;         //number of normal cooks
	int NumVeganCooks;         //number of vegan cooks
	int NumVipCooks;          //number of vip cooks

	int EventsNumber;
	int currentTimeStep;

public:

	Restaurant();

	~Restaurant();

	void ExecuteEvents(int TimeStep);	//executes all events at current timestep

	void RunSimulation();

	void CancelOrder(int r_ID);

	void PromoteOrder(int Oid);         //to be implemented in phase 2

	void FillDrawingList(int CurrentTimeStep);

	int GetNumNormal();

	int GetNumVegan();
	
	int GetNumVip();

	void ReadFile();

	void PrintFile();

	void AddToNormalList(NormalOrder* po);  //adding orders to different data structures

	void AddToVeganList(VeganOrder* po);

	void AddToVipList(VipOrder* po);

	void RunInteractive();               //for simulation

	void RunStepByStep();            

	void AssignOrders();           
};
#endif
