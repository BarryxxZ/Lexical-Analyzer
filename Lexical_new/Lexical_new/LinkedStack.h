#pragma once
#ifndef LINKEDSTACK_H_
#define LINKEDSTACK_H_

#include"base.h"
/*
	The simple chained stack defined by myself
	need a simple stack to store different data type
	It is similar to: LinkedStack ==> stack<StackNode>
*/
template <class T>
class LinkedStack;

template <class T>
class StackNode
{
	friend class LinkedStack<T>;
private:
	T data;
	StackNode<T> *next;

	StackNode(T item = 0, StackNode<T> *p = NULL)
	{
		data = item;
		next = p;
	}
};

template <class T>
class LinkedStack
{
private:
	StackNode<T> *top;
public:
	LinkedStack();
	~LinkedStack();

	bool IsEmpty() const;

	void Push(const T &item);

	T Pop();

	T Front();

	void Clear();
};

#endif
