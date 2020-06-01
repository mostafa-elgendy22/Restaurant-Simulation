#pragma once
#include "Generic_DS/Node.h"

template<class T, class K>  // K is for searching using a key value
class LinkedSortedList
{
	Node<T>* Head;	//Pointer to the head of the list

	Node<T>* Back;  //Pointer to the back of the list

	int count;     //Number of the elements of the list

public:

	LinkedSortedList();
	~LinkedSortedList();
	bool isEmpty();
	int GetLength();
	void Insert(const T& item);
	void Clear();
	bool Delete(T& item);
	bool peekHead(T& frontEntry);
	T* toArray();
	bool GetEntry(T& item, K key);
};


template<class T, class K>
LinkedSortedList<T, K>::LinkedSortedList()
{
	Head = nullptr;
	Back = nullptr;
	count = 0;
}

template<class T, class K>
LinkedSortedList<T, K>::~LinkedSortedList()
{
	Clear();
}

template<class T, class K>
bool LinkedSortedList<T, K>::isEmpty()
{
	return count == 0;
}


template<class T, class K>
void LinkedSortedList<T, K>::Insert(const T& item)
{
	Node<T>* ptr = new Node<T>(item);

	if (!Head)
	{
		Head = ptr;
		Back = Head;
		Head->setNext(nullptr);
		count++;
		return;
	}

	if (*(Back->getItem()) > * (ptr->getItem()))
	{
		Back->setNext(ptr);
		Back = ptr;
		count++;
		return;
	}

	if (*(ptr->getItem()) > * (Head->getItem()))
	{
		ptr->setNext(Head);
		Head = ptr;
		count++;
		return;
	}

	Node<T>* prev = Head;
	Node<T>* current = Head->getNext();
	while (current)
	{
		if (*(ptr->getItem()) > * (current->getItem()))
		{
			ptr->setNext(current);
			prev->setNext(ptr);
			count++;
			return;
		}
		prev = current;
		current = current->getNext();
	}
	prev->setNext(ptr);
	Back = ptr;
	count++;
}

template<class T, class K>
void LinkedSortedList<T, K>::Clear()
{
	while (Head)
	{
		Node<T>* P = Head;
		Head = Head->getNext();
		delete P;
		P = nullptr;
	}
	count = 0;
	Head = Back = nullptr;
}

template<class T, class K>
bool LinkedSortedList<T, K>::Delete(T& item)
{
	if (!Head)
		return false;

	Node<T>* todelete;

	if (Head->getItem() == item)
	{
		if (Head == Back)
		{
			Back = nullptr;
		}
		todelete = Head;
		Head = Head->getNext();
		delete todelete;
		todelete = nullptr;
		count--;
		return true;
	}

	Node<T>* ptr = Head;
	todelete = Head->getNext();
	while (todelete)
	{
		if (todelete->getItem() == item)
		{
			if (Back == todelete)
			{
				Back = ptr;
			}
			ptr->setNext(todelete->getNext());
			delete todelete;
			todelete = nullptr;
			count--;
			return true;
		}
		ptr = ptr->getNext();
		todelete = todelete->getNext();
	}
	return false;
}

template<class T, class K>
bool LinkedSortedList<T, K>::peekHead(T& frontEntry)  //to get the head
{
	if (isEmpty())
		return false;

	frontEntry = Head->getItem();
	return true;

}

template<class T, class K>
T* LinkedSortedList<T, K>::toArray()
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



template<class T, class K>
int LinkedSortedList<T, K>::GetLength()
{
	return count;
}

template<class T, class K>
bool LinkedSortedList<T, K>::GetEntry(T& item, K key)
{
	if (!Head)
	{
		return false;
	}

	K search_key = *(Back->getItem());
	if (search_key == key)
	{
		item = Back->getItem();
		return true;
	}
	Node<T>* ptr = Head;
	while (ptr)
	{
		search_key = *(ptr->getItem());
		if (search_key == key)
		{
			item = ptr->getItem();
			return true;
		}
		ptr = ptr->getNext();
	}
	return false;
}