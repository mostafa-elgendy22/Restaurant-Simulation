#pragma once

#include "Generic_DS/Node.h"


template<typename T>
class PriorityQueue
{
private:

	Node<T>* Front;

public:

	PriorityQueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry);
	bool dequeue(T& frntEntry);
	bool peekFront(T& frntEntry)  const;
	T* toArray(int& count);	//returns array of T (array of items)
	~PriorityQueue();
};



template<typename T>
PriorityQueue<T>::PriorityQueue()
{
	Front = nullptr;
}

template<typename T>
bool PriorityQueue<T>::isEmpty() const
{
	return Front == nullptr;
}

template<typename T>
bool PriorityQueue<T>::enqueue(const T& newEntry)
{
	Node<T>* newnode = new Node<T>(newEntry);

	if (Front == nullptr)
	{
		Front = newnode;
		return true;
	}

	if (*(newnode->getItem()) > * (Front->getItem()))
	{
		newnode->setNext(Front);
		Front = newnode;
		return true;
	}

	Node<T>* ptr = Front->getNext();
	Node<T>* prev = Front;
	while (ptr)
	{
		if (*(newnode->getItem()) > * (ptr->getItem()))
		{
			newnode->setNext(ptr);
			prev->setNext(newnode);
			return true;
		}
		ptr = ptr->getNext();
		prev = prev->getNext();
	}
	prev->setNext(newnode);  //if the new node has the least priority
	return true;
}


template<typename T>
bool PriorityQueue<T>::dequeue(T& frontEntry)
{
	if (isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = Front;
	frontEntry = Front->getItem();
	Front = Front->getNext();

	delete nodeToDeletePtr;

	return true;
}

template<typename T>
bool PriorityQueue<T>::peekFront(T& frontEntry)  const
{
	if (isEmpty())
		return false;

	frontEntry = Front->getItem();
	return true;
}



template<typename T>
PriorityQueue<T>::~PriorityQueue()
{
	while (Front)
	{
		Node<T>* temp = Front;
		Front = Front->getNext();
		delete temp;
		temp = nullptr;
	}
}


template <typename T>
T* PriorityQueue<T>::toArray(int& count)
{
	count = 0;

	if (!Front)
		return nullptr;
	//counting the no. of items in the Queue
	Node<T>* p = Front;


	while (p)
	{
		p = p->getNext();
		count++;
	}


	T* Arr = new T[count];
	p = Front;
	for (int i = 0; i < count; i++)
	{
		Arr[i] = p->getItem();
		p = p->getNext();
	}
	return Arr;
}