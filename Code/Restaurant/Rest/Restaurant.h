#ifndef __RESTAURANT_H_

#define __RESTAURANT_H_



#include "..\Defs.h"

#include "..\CMUgraphicsLib\CMUgraphics.h"

#include "..\GUI\GUI.h"

#include "..\Generic_DS\Queue.h"

#include "..\Events\Event.h"

#include"../PriorityQueue.h"

#include "../LInkedList.h"

#include "Order.h"


class Restaurant
{

private:

	GUI* pGUI;

	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file


	//our data structures 


	//lists of orders

	priorityqueue<Order*>VipOrders;

	LinkedList<Order*>NormalOrders;

	Queue<Order*>VeganOrders;

	/***/

	LinkedList<Order*>InServiceOrders;

	Queue<Order*>FinishedOrders;

	//lists of cooks

	Queue<Cook*>NormalCooks;

	Queue<Cook*>VeganCooks;

	Queue<Cook*>VipCooks;

	/**/

	// to be used in phase 2

	Queue<Cook*>NormalInBreak;

	Queue<Cook*>VeganInBreak;

	Queue<Cook*>VipInBreak;

	/***/

	int maxCooks;           //number of orders a cook must prepare before taking a break

	int NumNormalCooks;        //number of normal cooks

	int NormalSpeed;        //speed of normal cooks

	int NormalBreak;        //break duration of normal cooks

	int NumVeganCooks;         //number of vegan cooks

	int VeganSpeed;         //speed of vegan cooks

	int VeganBreak;         //break duration of vegan cooks

	int NumVipCooks;           //number of vip cooks

	int VipSpeed;           //speed of vip cooks

	int VipBreak;           //break duration of vip cooks

	int AutoPromoted;       //the number of time steps after which an order is automaticallt promoted to vip

	int EventsNumber;

public:

	Restaurant();

	~Restaurant();

	void ExecuteEvents(int TimeStep);	//executes all events at current timestep

	void RunSimulation();

	void cancelorder(int r_ID);
	 
	void PromoteOrder(int Oid);         //to be implemented in phase 2

	void FillDrawingList(int CurrentTimeStep);

	int GetNumNormal();

	int GetNumVegan();
	
	int GetNumVip();
	
	void ReadFile();

	void addtonormarlist(Order* po);  //adding orders to different data structures

	void addtoveganlist(Order* po);

	void addtoviplist(Order* po);

	void simulate();
};
#endif
