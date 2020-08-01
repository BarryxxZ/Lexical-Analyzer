#include"LinkedStack.h"

template <class T>
LinkedStack<T>::LinkedStack()
{
	top = NULL;
}

template <class T>
LinkedStack<T>::~LinkedStack()
{
	Clear();
}

template <class T>
bool LinkedStack<T>::IsEmpty() const
{
	return (!top);
}

template <class T>
void LinkedStack<T>::Push(const T &item)
{
	top = new StackNode<T>(item, top);
}

template <class T>
T LinkedStack<T>::Pop()
{
	if (!IsEmpty())
	{
		StackNode<T> *temp = top;
		top = top->next;
		T value = temp->data;
		delete temp;
		return value;
	}
	else
	{
		cout << "Stack is empty!" << endl;
		_getch();
		exit(1);
	}
}

template <class T>
T LinkedStack<T>::Front()
{
	if (!IsEmpty())
	{
		return top->data;
	}
	else
	{
		cout << "stack is empty!" << endl;
		_getch();
		exit(1);
	}
}

template <class T>
void LinkedStack<T>::Clear()
{
	StackNode<T> *temp = new StackNode<T>();
	while (top)
	{
		temp = top;
		top = top->next;
		delete temp;
	}
}