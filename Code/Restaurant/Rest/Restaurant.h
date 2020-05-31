#pragma once

#include "Order.h"
#include "../OrderService.h"
#include "../Defs.h"
#include "../CMUgraphicsLib/CMUgraphics.h"
#include "../GUI/GUI.h"
#include "../Generic_DS/Queue.h"
#include "../Events/Event.h"
#include "../PriorityQueue.h"
#include "../LinkedList.h"
#include "../LinkedSortedList.h"
#include "../BinarySearchTree.h"
#include "../VipOrder.h"
#include "../VeganOrder.h"
#include "../NormalOrder.h"



class Restaurant
{
private:

	GUI* pGUI;
	Queue<Event*> EventsQueue;	   //Queue of all events that will be loaded from the input file

	LinkedSortedList<VipOrder*, int> VipOrders;
	LinkedList<NormalOrder*, int> NormalOrders;
	Queue<VeganOrder*> VeganOrders;

	BinarySearchTree<OrderService*, int>InServiceOrders;
	LinkedList<Order*, int>FinishedOrders;

	Queue<Cook*>NormalCooks;
	Queue<Cook*>VeganCooks;
	Queue<Cook*>VipCooks;

	PriorityQueue<Cook*>NormalInBreak;
	Queue<Cook*>NormalInRest;

	PriorityQueue<Cook*>VeganInBreak;
	Queue<Cook*>VeganInRest;

	PriorityQueue<Cook*>VipInBreak;
	Queue<Cook*>VipInRest;


	int NumNormalCooks;         //number of normal cooks
	int NumVeganCooks;         //number of vegan cooks
	int NumVipCooks;          //number of vip cooks

	int NumAutoPromoted;        //auto promoted orders (from normal to vip)

	int NumNormalOrders;          //number of normal orders
	int NumVeganOrders;          //number of vegan orders
	int NumVipOrders;           //number of vip orders

	float InjProb;
	int NumUrgentOrders;
	int NumInjuredCooks;


public:

	Restaurant();

	~Restaurant();

	void ReadFile();

	void PrintFile();

	void RunSimulation();

	void Run(PROG_MODE mode);

	void ExecuteEvents(int TimeStep);	//executes all events at current timestep

	NormalOrder*& GetNormalOrderFromID(int ID);

	void CancelOrder(int ID);

	void PromoteOrder(int ID,int time);

	void FillDrawingList(int currentTimeStep);

	void AddToNormalList(NormalOrder* po);

	void AddToVeganList(VeganOrder* po);

	void AddToVipList(VipOrder* po);

	Cook* FindCook(Order* ord);

	Cook* FindCookForUrgentOrder(VipOrder* pOrd);

	void AssignOrders(int currentTimeStep);

	void CompleteOrders(int currentTimeStep);

	void RemoveFromBreakList(int currentTimeStep);

	void ManageOrders(int currentTimeStep);

	void AddToInserviceList(OrderService* pServe);

	void AddToFinishedList(Order* ord);

	void AddToBreakList(Cook* pCook);

	void AddToCookList(Cook* pCook);

	void CheckInjuries(int currentTimeStep);

	void RemoveFromRestList(int currentTimeStep);

	void AddToRestList(Cook* pCook);
};