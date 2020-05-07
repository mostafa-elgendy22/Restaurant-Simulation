#pragma once

template<class T,class K>
class PriorityQueueNode
{
private:

	T item;
	PriorityQueueNode<T, K>* next;
	K priority;

public:

	PriorityQueueNode(T data, K p, PriorityQueueNode<T, K>* Next = nullptr)
	{
		item = data;
		priority = p;
		next = Next;
	}
	T getItem()
	{
		return item;
	}
	void setItem(T data)
	{
		item = data;
	}
	void setNext(PriorityQueueNode<T, K>* Next)
	{
		next = Next;
	}
	PriorityQueueNode<T, K>* getNext()
	{
		return next;
	}
	void setPriority(K p)
	{
		priority = p;
	}
	K getPriority()
	{
		return priority;
	}
};