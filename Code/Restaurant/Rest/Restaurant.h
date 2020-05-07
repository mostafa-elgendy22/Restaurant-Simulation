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

	PriorityQueue<VipOrder*,float> VipOrders;
	LinkedList<NormalOrder*,int> NormalOrders;
	Queue<VeganOrder*> VeganOrders;

	LinkedList<Order*,int>InServiceOrders;
	LinkedList<Order*,int>FinishedOrders;

	Queue<Cook*>NormalCooks;
	Queue<Cook*>VeganCooks;
	Queue<Cook*>VipCooks;

	Queue<Cook*>NormalInBreak;
	Queue<Cook*>VeganInBreak;
	Queue<Cook*>VipInBreak;

	int NumNormalCooks;         //number of normal cooks
	int NumVeganCooks;         //number of vegan cooks
	int NumVipCooks;          //number of vip cooks

	int currentTimeStep;
	int NumAutoPromoted;

	int NumNormalOrders;         //number of normal orders
	int NumVeganOrders;          //number of vegan orders
	int NumVipOrders;           //number of vip orders

	int maxNumCooks; //number of orders a cook must prepare before taking a break
	int Vip_WT;
	float InjProb;
	int RstPrd;
	int NumUrgentOrders;
	int NumInjuredCooks;


public:

	Restaurant();

	~Restaurant();

	void ExecuteEvents(int TimeStep);	//executes all events at current timestep

	void RunSimulation();

	NormalOrder*& GetNormalOrderFromID(int ID);

	void CancelOrder(int ID);

	void PromoteOrder(int ID);       

	void FillDrawingList();

	void ReadFile();

	void PrintFile();

	void AddToNormalList(NormalOrder* po);  //adding orders to different data structures

	void AddToVeganList(VeganOrder* po);

	void AddToVipList(VipOrder* po);

	void RunInteractive();             

	void RunStepByStep();            

	void AssignOrders();           
};
#endif
