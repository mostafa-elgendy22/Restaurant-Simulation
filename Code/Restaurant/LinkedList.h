#ifndef _LINKEDLIST
#define _LINKEDLIST

#include "Generic_DS/Node.h"


template <typename T>
class LinkedList
{
private:
	Node<T>* Head;	//Pointer to the head of the list

	Node<T>* Back;

public:


	LinkedList()
	{
		Head = nullptr;
		Back = nullptr;
	}

	~LinkedList()
	{
		DeleteAll();
	}

	bool isempty()
	{
		return Head == nullptr;
	}

	void InsertBeg(const T& data)
	{
		if (!Head)
		{
			Head = new Node<T>(data);
			Back = Head;
			return;
		}
		Node<T>* R = new Node<T>(data);
		R->setNext(Head);
		Head = R;
	}

	void DeleteAll()
	{
		Node<T>* P = Head;
		while (Head)
		{
			P = Head->getNext();
			delete Head;
			Head = P;
		}
	}

	void deletebyid(int r_ID)
	{
		if (!Head)
			return;

		Node<T>* todelete;

		if (Head->getID() == r_ID)
		{
			todelete = Head;
			Head = Head->getNext();
			delete todelete;
			return;
		}

		Node<T>* ptr = Head;

		while (ptr->getNext())
		{
			if (ptr->getNext()->getID() == r_ID)
			{
				todelete = ptr->getNext();
				ptr = ptr->getNext()->getNext();
				delete todelete;
				return;
			}
		}
		todelete = nullptr;

	}

	void InsertEnd(const T& data, int r_ID = -1)
	{
		Node<T>* New = new Node<T>(data, r_ID);

		if (!Head)
		{
			Head = New;
			Back = Head;
			Head->setNext(nullptr);
			return;
		}
		if (Head == Back)
		{
			Head->setNext(New);
			Back = New;
			return;
		}
		Back->setNext(New);
		Back = New;
	}

	bool dequeue(T& frntEntry)  //to get the head
	{
		if (isempty())
			return false;

		Node<T>* nodeToDeletePtr = Head;
		frntEntry = Head->getItem();
		Head = Head->getNext();
		delete nodeToDeletePtr;
		return true;

	}

	T* toArray(int& count)
	{
		count = 0;

		if (!Head)
			return nullptr;
		//counting the no. of items in the Queue
		Node<T>* p = Head;
		while (p)
		{
			count++;
			p = p->getNext();
		}


		T* Arr = new T[count];
		p = Head;
		for (int i = 0; i < count; i++)
		{
			Arr[i] = p->getItem();
			p = p->getNext();
		}
		return Arr;
	}

	bool SearchByID(int ID, T& entry) //to get orders by their id
	{
		if (Head && Head->getID() == ID)
		{
			entry = Head->getItem();
			return true;
		}
		if (Back && Back->getID() == ID)
		{
			entry = Back->getItem();
			return true;
		}
		Node<T>* ptr;
		if (Head)
		{
			ptr = Head->getNext();
		}
		while (ptr)
		{
			if (ptr->getID() == ID)
			{
				entry = ptr->getItem();
				return true;
			}
			ptr = ptr->getNext();
		}
		return false;
	}
};
#endif	
