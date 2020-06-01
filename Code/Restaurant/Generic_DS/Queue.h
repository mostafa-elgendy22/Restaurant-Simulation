#ifndef __QUEUE_H_
#define __QUEUE_H_


#include "Node.h"

template <typename T>
class Queue
{
private:

	Node<T>* Front;
	Node<T>* Back;

public:
	Queue();
	bool isEmpty() const;
	bool enqueue(const T& newEntry);
	bool dequeue(T& frntEntry);
	bool peekFront(T& frntEntry)  const;
	T* toArray(int& count);	//returns array of T (array of items)
	~Queue();
};

template <typename T>
Queue<T>::Queue()
{
	Front = Back = nullptr;
}


template <typename T>
bool Queue<T>::isEmpty() const
{
	return Front == nullptr;
}


template <typename T>
bool Queue<T>::enqueue(const T& newEntry)
{
	Node<T>* newNodePtr = new Node<T>(newEntry);

	if (isEmpty())
		Front = newNodePtr;
	else
		Back->setNext(newNodePtr);
	Back = newNodePtr;
	return true;
}



template <typename T>
bool Queue<T>::dequeue(T& frntEntry)
{
	if (isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = Front;
	frntEntry = Front->getItem();
	Front = Front->getNext();

	if (nodeToDeletePtr == Back)
		Back = nullptr;


	delete nodeToDeletePtr;
	nodeToDeletePtr = nullptr;
	return true;
}


template <typename T>
bool Queue<T>::peekFront(T& frntEntry) const
{
	if (isEmpty())
		return false;

	frntEntry = Front->getItem();
	return true;
}


template <typename T>
Queue<T>::~Queue()
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
T* Queue<T>::toArray(int& count)
{
	count = 0;

	if (!Front)
		return nullptr;
	//counting the no. of items in the Queue
	Node<T>* p = Front;
	while (p)
	{
		count++;
		p = p->getNext();
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

#endif
