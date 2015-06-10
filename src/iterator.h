#pragma once
#include "node.h"

template <class T>
class Iterator
{
	template <class T>
	friend class List;

public:
	Iterator();
	void next();
	Iterator<T> operator++(int);
	void prev();
	Iterator<T> operator--(int);
	void set(Node<T> *node);
	Node<T>* get();

private:
	Node<T> *pos;
};

template <class T>
Iterator<T>::Iterator()
{
	pos = nullptr;
}

template <class T>
void Iterator<T>::next()
{
	pos = pos->next;
}

template <class T>
Iterator<T> Iterator<T>::operator++(int)
{
	pos = pos->next;
	return *this;
}

template <class T>
void Iterator<T>::prev()
{
	pos = pos->prev;
}

template <class T>
Iterator<T> Iterator<T>::operator--(int)
{
	pos = pos->prev;
	return *this;
}

template <class T>
void Iterator<T>::set(Node<T> *node)
{
	pos = node;
}

template <class T>
Node<T>* Iterator<T>::get()
{
	return pos;
}
