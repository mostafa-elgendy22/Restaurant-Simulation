#pragma once

#include"PriorityQueueNode.h"


template<typename T, typename K>
class PriorityQueue
{
private:

	PriorityQueueNode<T, K>* Front;

public:

	PriorityQueue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry, K priority_key);
	bool dequeue(T& frntEntry);
	bool peekFront(T& frntEntry)  const;
	T* toArray(int& count);	//returns array of T (array of items)
	~PriorityQueue();
};



template<typename T, typename K>
PriorityQueue<T, K>::PriorityQueue()
{
	Front = nullptr;
}

template<typename T, typename K>
bool PriorityQueue<T, K>::isEmpty() const
{
	return Front == nullptr;
}

template<typename T, typename K>
bool PriorityQueue<T, K>::enqueue(const T& newEntry, K priority_key)
{
	PriorityQueueNode<T, K>* newnode = new PriorityQueueNode<T, K>(newEntry, priority_key);

	if (Front == nullptr)
	{
		Front = newnode;
		return true;
	}


	if (newnode->getPriority() > Front->getPriority())
	{
		newnode->setNext(Front);
		Front = newnode;
		return true;
	}

	PriorityQueueNode<T, K>* ptr = Front->getNext();
	PriorityQueueNode<T, K>* prev = Front;
	while (ptr)
	{
		if (ptr->getPriority() < newnode->getPriority())
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


template<typename T, typename K>
bool PriorityQueue<T, K>::dequeue(T& frontEntry)
{
	if (isEmpty())
		return false;

	PriorityQueueNode<T, K>* nodeToDeletePtr = Front;
	frontEntry = Front->getItem();
	Front = Front->getNext();

	delete nodeToDeletePtr;

	return true;
}

template<typename T, typename K>
bool PriorityQueue<T, K>::peekFront(T& frontEntry)  const
{
	if (isEmpty())
		return false;

	frontEntry = Front->getItem();
	return true;
}



template<typename T, typename K>
PriorityQueue<T, K>::~PriorityQueue()
{
	while (Front)
	{
		PriorityQueueNode<T, K>* temp = Front;
		Front = Front->getNext();
		delete temp;
	}
}


template <typename T, typename K>
T* PriorityQueue<T, K>::toArray(int& count)
{
	count = 0;

	if (!Front)
		return nullptr;
	//counting the no. of items in the Queue
	PriorityQueueNode<T, K>* p = Front;
	PriorityQueueNode<T, K>* e = p;
	do
	{
		count++;
		p = p->getNext();
	} while ((p != e));


	T* Arr = new T[count];
	p = Front;
	for (int i = 0; i < count; i++)
	{
		Arr[i] = p->getItem();
		p = p->getNext();
	}
	return Arr;
}
