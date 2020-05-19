#pragma once

#include "BinaryTreeNode.h"

template<class T, class K = T>
class BinarySearchTree
{
	BinaryTreeNode<T>* Root;

	//utility functions

	static void rec_insert(T& item, BinaryTreeNode<T>*& subRoot);
	static BinaryTreeNode<T>* rec_delete(T& item, K key, BinaryTreeNode<T>* subRoot, bool& isDeleted, bool& entry);
	static BinaryTreeNode<T>* FindMin(BinaryTreeNode<T>* subRoot);
	static int rec_getcount(BinaryTreeNode<T>* root);
	static void rec_fillarrinorder(BinaryTreeNode<T>* root, T* Arr, int& indx);
	static void rec_get_entry(BinaryTreeNode<T>* subRoot, T& item, bool& isFound);

public:

	BinarySearchTree();
	void Insert(T& item);
	bool Delete(T& item, K key = -1);

	bool isEmpty();
	bool GetEntry(T& item);

	T* toArray(int& cnt);
};

template<class T, class K>
BinarySearchTree<T, K>::BinarySearchTree()
{
	Root = nullptr;
}

template<class T, class K>
bool BinarySearchTree<T, K>::isEmpty()
{
	return Root == nullptr;
}

template<class T, class K>
void BinarySearchTree<T, K>::rec_insert(T& item, BinaryTreeNode<T>*& subRoot)
{
	if (!subRoot)
	{
		subRoot = new BinaryTreeNode<T>(item);
	}
	else if (K(subRoot->GetItem()) <= K(item))
	{
		rec_insert(item, subRoot->GetRight());
	}
	else
	{
		rec_insert(item, subRoot->GetLeft());
	}
}

template<class T, class K>
void BinarySearchTree<T, K>::Insert(T& item)
{
	rec_insert(item, Root);
}

template<class T, class K>
BinaryTreeNode<T>* BinarySearchTree<T, K>::FindMin(BinaryTreeNode<T>* subRoot)
{
	if (!subRoot)
	{
		return nullptr;
	}
	if (!subRoot->GetLeft())
	{
		return subRoot;
	}
	return FindMin(subRoot->GetLeft());
}

template<class T, class K>
BinaryTreeNode<T>* BinarySearchTree<T, K>::rec_delete(T& item, K key, BinaryTreeNode<T>* subRoot, bool& isDeleted, bool& entry)
{
	if (!subRoot)
	{
		return nullptr;
	}

	if (!isDeleted)
	{
		K search_key = *(subRoot->GetItem());
		if (key < search_key)
		{
			subRoot->SetLeft(rec_delete(item, key, subRoot->GetLeft(), isDeleted, entry));
		}
		else if (key > search_key)
		{
			subRoot->SetRight(rec_delete(item, key, subRoot->GetRight(), isDeleted, entry));
		}
		else
		{
			if (!entry)
			{
				item = subRoot->GetItem();
				entry = true;
			}
			if (!subRoot->GetLeft() && !subRoot->GetRight())
			{
				delete subRoot;
				subRoot = nullptr;
			}
			else if (!subRoot->GetLeft())
			{
				BinaryTreeNode<T>* temp = subRoot;
				subRoot = subRoot->GetRight();
				delete temp;
			}
			else if (!subRoot->GetRight())
			{
				BinaryTreeNode<T>* temp = subRoot;
				subRoot = subRoot->GetLeft();
				delete temp;
			}
			else
			{
				BinaryTreeNode<T>* temp = FindMin(subRoot->GetRight());
				subRoot->SetItem(temp->GetItem());
				subRoot->SetRight(rec_delete(item, K(temp->GetItem()), subRoot->GetRight(), isDeleted, entry));
			}
			isDeleted = true;
			return subRoot;
		}
	}
}

template<class T, class K>
bool BinarySearchTree<T, K>::Delete(T& item, K key)
{
	bool isDeleted = false;
	bool entry = false;
	rec_delete(item, key, Root, isDeleted, entry);
	return isDeleted;
}

template<class T, class K>
void BinarySearchTree<T, K>::rec_get_entry(BinaryTreeNode<T>* subRoot, T& item, bool& isFound)
{
	if (!subRoot)
	{
		return;
	}
	bool flag = *(subRoot->GetItem());
	if (flag && !isFound)
	{
		item = subRoot->GetItem();
		isFound = true;
	}
	rec_get_entry(subRoot->GetLeft(), item, isFound);
	rec_get_entry(subRoot->GetRight(), item, isFound);
}

template<class T, class K>
bool BinarySearchTree<T, K>::GetEntry(T& item)
{
	bool isFound = false;
	rec_get_entry(Root, item, isFound);
	return isFound;
}

template<class T, class K>
int BinarySearchTree<T, K>::rec_getcount(BinaryTreeNode<T>* root)
{
	if (!root)return 0;
	return 1 + rec_getcount(root->GetLeft()) + rec_getcount(root->GetRight());
}

template<class T, class K>
void BinarySearchTree<T, K>::rec_fillarrinorder(BinaryTreeNode<T>* root, T* Arr, int& indx)
{
	if (!root)return;
	rec_fillarrinorder(root->GetLeft(), Arr, indx);
	Arr[indx++] = root->GetItem();
	rec_fillarrinorder(root->GetRight(), Arr, indx);

}

template<class T, class K>
T* BinarySearchTree<T, K>::toArray(int& cnt)
{
	cnt = rec_getcount(Root);
	T* Arr = new T[cnt];
	int indx = 0;
	rec_fillarrinorder(Root, Arr, indx);
	return Arr;
}