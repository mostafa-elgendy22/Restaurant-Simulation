#ifndef __ORDER_H_
#define __ORDER_H_

#include "..\Defs.h"


class Order
{

protected:

	int ID;              //Each order has a unique ID (from 1 --> 999 )

	ORD_TYPE type;		 //order type: Normal, vegan, VIP

	ORD_STATUS status;	 //waiting, in-service, done

	int Size;	         //number of dishes

	double totalMoney;	  //Total order money

	int ArrTime, ServTime, FinishTime;	//arrival, service start, and finish times


public:

	Order(int ID, ORD_TYPE r_Type, int size, int time, double money);

	virtual ~Order();

	int GetID();

	ORD_TYPE GetType() const;

	void SetSize(int s);

	int GetSize() const;

	void SetStatus(ORD_STATUS s);

	ORD_STATUS getStatus() const;

	void SetType(ORD_TYPE typ);

	bool operator == (const Order*& ord);

	int GetAT();

	int GetST();

	int GetFT();

	double GetMoney();

	void SetMoney(double money);

	void SetST(int time);

	void SetFT(int time);

	bool operator > (Order& ord);

};

#endif

