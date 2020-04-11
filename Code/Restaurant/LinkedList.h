#ifndef _LINKEDLIST
#define _LINKEDLIST

#include "Generic_DS/Node.h"
#include"iostream"
using namespace std;

template <typename T>
class LinkedList
{
private:
	Node<T>* Head;	//Pointer to the head of the list
public:


	LinkedList()
	{
		Head = nullptr;
	}

	~LinkedList()
	{
		DeleteAll();
	}

	bool isempty()
	{
		return Head == nullptr;
	}

	void PrintList()	const
	{
		Node<T>* p = Head;
		while (p)
		{
			cout << p->getItem() << " ";
			p = p->getNext();
		}
	}

	void InsertBeg(const T& data)
	{
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
		if (!Head)return;

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
		Node<T>* ptr = Head;
		Node<T>* New = new Node<T>(data, r_ID);
		New->setItem(data);
		if (!Head)
		{
			Head = New;
			Head->setNext(nullptr);
			return;
		}
		while (ptr->getNext())
			ptr = ptr->getNext();

		ptr->setNext(New);
		New->setNext(nullptr);

	}

	bool dequeue(T& frntEntry)
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

};

#endif	
