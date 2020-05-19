#pragma once

template<class T>
class BinaryTreeNode
{
	T item;
	BinaryTreeNode<T>* left;
	BinaryTreeNode<T>* right;

public:

	BinaryTreeNode(T data)
	{
		item = data;
	}

	void SetItem(T data)
	{
		item = data;
	}
	T GetItem()
	{
		return item;
	}
	void SetRight(BinaryTreeNode<T>* r)
	{
		right = r;
	}
	BinaryTreeNode<T>*& GetRight()
	{
		return right;
	}
	void SetLeft(BinaryTreeNode<T>* l)
	{
		left = l;
	}
	BinaryTreeNode<T>*& GetLeft()
	{
		return left;
	}
};