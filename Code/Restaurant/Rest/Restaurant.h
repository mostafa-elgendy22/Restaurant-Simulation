#ifndef __RESTAURANT_H_

#define __RESTAURANT_H_



#include "..\Defs.h"

#include "..\CMUgraphicsLib\CMUgraphics.h"

#include "..\GUI\GUI.h"

#include "..\Generic_DS\Queue.h"

#include "..\Events\Event.h"

#include"../PriotiyQueue.h"

#include "../LInkedList.h"

#include "Order.h"



// it is the maestro of the project

class Restaurant

{

private:

	GUI* pGUI;

	Queue<Event*> EventsQueue;	//Queue of all events that will be loaded from file





	/// ==> 

	//	DEMO-related members. Should be removed in phases 1&2

		Queue<Order*> DEMO_Queue;	//Important: This is just for demo

	/// ==>


	//our data structures for orders
	priorityqueue<Order*>viporders;
	
	LinkedList<Order*>normalorders;

	Queue<Order*>veganorders;

	LinkedList<Order*>inservice;

	LinkedList<Order*>finished;

	LinkedList<Cook*>normalcooks;

	LinkedList<Cook*>vegancooks;

	LinkedList<Cook*>vipcooks;


	//

	// TODO: Add More Data Members As Needed

	//



	int maxCooks;           //number of orders a cook must prepare before taking a break

	int NormalCooks;        //number of normal cooks

	int NormalSpeed;        //speed of normal cooks

	int NormalBreak;        //break duration of normal cooks

	int VeganCooks;         //number of vegan cooks

	int VeganSpeed;         //speed of vegan cooks

	int VeganBreak;         //break duration of vegan cooks

	int VipCooks;           //number of vip cooks

	int VipSpeed;           //speed of vip cooks

	int VipBreak;           //break duration of vip cooks

	int AutoPromoted; //the number of time steps after which an order is automaticallt promoted to vip

	int EventsNumber;

public:



	Restaurant();

	~Restaurant();



	void ExecuteEvents(int TimeStep);	//executes all events at current timestep

	void RunSimulation();

	void cancelorder(int r_ID);



	void FillDrawingList(int CurrentTimeStep);



	//

	// TODO: Add More Member Functions As Needed

	//

	int GetNumNormal();
	int GetNumVegan();
	int GetNumVip();
	void ReadFile();

	/// ===================    DEMO-related functions. Should be removed in phases 1&2   ================= 



	void Just_A_Demo();	//just to show a demo and should be removed in phase1 1 & 2
	void AddtoDemoQueue(Order* po);	//adds an order to the demo queue

	void addtonormarlist(Order*po);  //adding orders to different datastructures
	void addtoveganlist(Order* po);
	void addtoviplist(Order* po);
	void simulate();
/// ================================================================================================== 







};



#endif
