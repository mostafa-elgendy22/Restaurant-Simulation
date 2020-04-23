#ifndef _LINKEDLIST
#define _LINKEDLIST

#include "Generic_DS/Node.h"


template <typename T>
class LinkedList
{
private:
	Node<T>* Head;	//Pointer to the head of the list

	Node<T>* Back;  //Pointer to the back of the list

	int count;
public:

	LinkedList()
	{
		Head = nullptr;
		Back = nullptr;
		count = 0;
	}

	~LinkedList()
	{
		DeleteAll();
	}

	bool isEmpty()
	{
		return count == 0;
	}

	void InsertEnd(const T& data)
	{
		Node<T>* ptr = new Node<T>(data);

		if (!Head)
		{
			Head = ptr;
			Back = Head;
			Head->setNext(nullptr);
			count++;
			return;
		}
		if (Head == Back)
		{
			Head->setNext(ptr);
			Back = ptr;
			count++;
			return;
		}
		Back->setNext(ptr);
		Back = ptr;
		count++;
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
		count = 0;
	}

	void DeleteByID(int ID)
	{
		if (!Head)
			return;

		Node<T>* todelete;

		if (Head->getItem()->GetID() == ID)
		{
			todelete = Head;
			Head = Head->getNext();
			delete todelete;
			count--;
			return;
		}

		Node<T>* ptr = Head;

		while (ptr->getNext())
		{
			if (ptr->getNext()->getItem()->GetID() == ID)
			{
				todelete = ptr->getNext();
				ptr = ptr->getNext()->getNext();
				delete todelete;
				count--;
				return;
			}
		}
		todelete = nullptr;
	}

	bool head(T& frntEntry)  //to get the head
	{
		if (isEmpty())
			return false;

		Node<T>* nodeToDeletePtr = Head;
		frntEntry = Head->getItem();
		Head = Head->getNext();
		delete nodeToDeletePtr;
		return true;

	}

	T* toArray(int& count)
	{

		if (!Head)
			return nullptr;

		Node<T>* p;
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

	void Sort()
	{

	}
	
	int GetCount()
	{
		return count;
	}
};
#endif	
