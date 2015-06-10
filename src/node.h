#pragma once
#include <time.h>
#include <Windows.h>

template <class T>
class Node
{
	template <class T>
	friend class List;
	template <class T>
	friend class Iterator;

public:
	Node(T);
	~Node();
	T & getData();
	void setData(T);
	inline float getTime();

private:
	T data;
	Node<T> *next;
	Node<T> *prev;
	float allocation_time;
};

template <class T>
Node<T>::Node(T object)
{
	data = object;
	next = nullptr;
	prev = nullptr;
	//Sleep(1);
	allocation_time = (float)clock() / CLOCKS_PER_SEC;
}

template <class T>
Node<T>::~Node()
{

}

template <class T>
T & Node<T>::getData()
{
	return data;
}

template <class T>
void Node<T>::setData(T object)
{
	data = object;
}

template <class T>
inline float Node<T>::getTime()
{
	return allocation_time;
}
