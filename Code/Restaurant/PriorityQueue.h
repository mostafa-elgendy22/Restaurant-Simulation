#pragma once

#include"Generic_DS/Node.h"


template<typename T>
class PriorityQueue
{
private:

	Node<T>* backPtr;

public:
	PriorityQueue();
	bool isEmpty() const;
	virtual bool enqueue(const T& newEntry);
	bool dequeue(T& frntEntry);
	bool peekFront(T& frntEntry)  const;
	T* toArray(int& count);	//returns array of T (array of items)
	~PriorityQueue();
};



template<typename T>
PriorityQueue<T>::PriorityQueue()
{
	backPtr = nullptr;
}

template<typename T>
bool PriorityQueue<T>::isEmpty() const
{
	return backPtr == nullptr;
}

template<typename T>
bool PriorityQueue<T>::enqueue(const T& newEntry)
{
	Node<T>* newnode = new Node<T>(newEntry);

	if(backPtr==nullptr)
	{
		backPtr = newnode;
		backPtr->setNext(newnode);
		return true;
	}
	
	
	Node<T>* prev = backPtr;
	Node<T>* ptr = backPtr->getNext();

	while (ptr != backPtr)
	{
		if (*(newnode->getItem()) >*(ptr->getItem()))
		{
			prev->setNext(newnode);
			newnode->setNext(ptr);
			return true;
		}
		prev = ptr;
		ptr = ptr->getNext();
	}

	if (*(newnode->getItem())> *(backPtr->getItem()))
	{
		prev->setNext(newnode);
		newnode->setNext(backPtr);
	}
	else
	{	
		newnode->setNext(backPtr->getNext());
		backPtr->setNext(newnode);
		backPtr = newnode;
	}

	
	return true;
}


template<typename T>
bool PriorityQueue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())return false;

	Node<T>* nodeToDeletePtr = backPtr->getNext();
	frntEntry = backPtr->getNext()->getItem();
	backPtr->setNext(nodeToDeletePtr->getNext());

	if (backPtr == nodeToDeletePtr)
		backPtr = nullptr;

	//delete nodeToDeletePtr;

	return true;
}

template<typename T>
bool PriorityQueue<T>::peekFront(T& frntEntry)  const
{
	if (isEmpty())
		return false;

	frntEntry = backPtr->getItem();
	return true;
}



template<typename T>
PriorityQueue<T>::~PriorityQueue()
{
	
}


template <typename T>
T* PriorityQueue<T>::toArray(int& count)
{
	count = 0;

	if (!backPtr)
		return nullptr;
	//counting the no. of items in the Queue
	Node<T>* p = backPtr->getNext();
	Node<T>* e = p;
	do
	{
		count++;
		p = p->getNext();
	}while ((p != e));


	T* Arr = new T[count];
	p = backPtr->getNext();
	for (int i = 0; i < count; i++)
	{
		Arr[i] = p->getItem();
		p = p->getNext();
	}
	return Arr;
}
