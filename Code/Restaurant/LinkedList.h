#ifndef _LINKEDLIST
#define _LINKEDLIST

#include "Generic_DS/Node.h"


template <class T, class K = T>    // K is for searching using a key value
class LinkedList
{
private:

	Node<T>* Head;	//Pointer to the head of the list

	Node<T>* Back;  //Pointer to the back of the list

	int count;     //Number of the elements of the list

public:

	LinkedList();
	~LinkedList();
	bool isEmpty();
	int GetLength();
	void InsertEnd(const T& data);
	void Clear();
	bool Delete(T& item);
	bool head(T& frontEntry);
	T* toArray();
	bool GetEntry(T& item,K key);
	void Sort();
};
#endif	


template<class T, class K>
LinkedList<T, K>::LinkedList()
{
	Head = nullptr;
	Back = nullptr;
	count = 0;
}

template<class T, class K>
LinkedList<T, K>::~LinkedList()
{
	Clear();
}

template<class T, class K>
bool LinkedList<T, K>::isEmpty()
{
	return count == 0;
}

template<class T, class K>
void LinkedList<T, K>::InsertEnd(const T& data)
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

template<class T, class K>
void LinkedList<T, K>::Clear()
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

template<class T, class K>
bool LinkedList<T, K>::Delete(T& item)
{
	if (!Head)
		return false;

	Node<T>* todelete;

	if (Head->getItem() == item)
	{
		todelete = Head;
		Head = Head->getNext();
		delete todelete;
		todelete = nullptr;
		count--;
		return true;
	}

	Node<T>* ptr = Head->getNext();

	while (ptr)
	{
		if (ptr->getItem() == item)
		{
			todelete = ptr->getNext();
			ptr = ptr->getNext()->getNext();
			delete todelete;
			todelete = nullptr;
			count--;
			return true;
		}
	}
	return false;
}

template<class T, class K>
bool LinkedList<T,K>::head(T& frontEntry)  //to get the head
{
	if (isEmpty())
		return false;

	Node<T>* nodeToDeletePtr = Head;
	frontEntry = Head->getItem();
	Head = Head->getNext();
	delete nodeToDeletePtr;
	return true;

}

template<class T,class K>
T* LinkedList<T,K>::toArray()
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



template<class T,class K>
void LinkedList<T, K>::Sort()
{

}

template<class T,class K>
int LinkedList<T, K>::GetLength()
{
	return count;
}

template<class T,class K>
bool LinkedList<T, K>::GetEntry(T& item, K key)
{
	if (!Head)
	{
		return false;
	}

	if (K(Head->getItem()) == key)
	{
		item = Head->getItem();
		return true;
	}
	Node<T>* ptr = Head->getNext();
	while (ptr)
	{
		if (K(ptr->getItem()) == key)
		{
			item = ptr->getItem();
			return true;
		}
		ptr = ptr->getNext();
	}
	return false;
}