#pragma once
#include "iterator.h"

template <class T>
class List
{
public:
	List();
	List(T);
	List(const List<T> &list);
	~List();
	T & operator[](size_t element);
	bool push_back(T);
	bool push_front(T);
	T pop_back();
	T pop_front();
	T erase(Iterator<T> &it);
	bool insert(T, Iterator<T> &it);
	Iterator<T> begin();
	Iterator<T> end();

private:
	Node<T> *first;
	Node<T> *last;
	size_t size;
};

template <class T>
List<T>::List()
{
	first = nullptr;
	last = nullptr;
	size = 0;
}

template <class T>
List<T>::List(T object)
{
	first = new Node<T>(object);
	last = first;
	size = 1;
}

template <class T>
List<T>::List(const List<T> &list)
{
	Iterator<T> it, it_copy;
	it.set(list.first);
	first = new Node<T>(it.get()->getData());
	it_copy = begin();
	while (it.get() != list.last)
	{
		it++;
		it_copy.get()->next = new Node<T>(it.get()->getData());
		Node<T> *tmp = it_copy.get();
		it_copy++;
		it_copy.get()->prev = tmp;
	}
	last = it_copy.get();
}

template <class T>
List<T>::~List()
{
	Iterator<T> it;
	it.set(first);
	if (last != first)
	{
		while (it.get() != last)
		{
			it++;
			delete it.get()->prev;
		}
		delete last;
	}
	else delete it.get();
}

template <class T>
T & List<T>::operator[](size_t element)
{
	element++;
	Iterator<T> it = begin();
	while (element > 1)
	{
		it++;
		element--;
	}
	return it.get()->getData();
}

template <class T>
bool List<T>::push_back(T object)
{
	Node<T>* new_node = new Node<T>(object);
	if (size > 0)
	{
		last->next = new_node;
		new_node->prev = last;
		last = new_node;
		new_node->next = nullptr;

	}
	else
	{
		first = new_node;
		last = new_node;
	}
	size++;
	return 0;
}

template <class T>
bool List<T>::push_front(T object)
{
	Node<T>* new_node = new Node<T>(object);
	if (size > 0)
	{
		first->prev = new_node;
		new_node->next = first;
		first = new_node;
		new_node->prev = nullptr;
	}
	else
	{
		first = new_node;
		last = new_node;
	}
	size++;
	return 0;
}

template <class T>
T List<T>::pop_back()
{
	T deleted = last->getData();
	if (last != first)
	{
		last = last->next;
	}
	if (last->prev != nullptr)
		delete last->prev;
	last->next = nullptr;
	size--;
	return deleted;
}

template <class T>
T List<T>::pop_front()
{
	T deleted = first->getData();
	if (first != last)
	{
		first = first->next;
	}
	if (first->prev != nullptr)
		delete first->prev;
	first->prev = nullptr;
	size--;
	return deleted;
}

template <class T>
T List<T>::erase(Iterator<T> &it)
{
	T deleted = it.get()->getData();
	Node<T>* to_delete = it.get();
	if (it.get() == first)
	{
		it++;
		delete it.get()->prev;
		it.get()->prev = nullptr;
		first = it.get();
	}
	else
	{
		if (it.get() == last)
		{
			it--;
			delete it.get()->next;
			it.get()->next = nullptr;
			last = it.get();
		}
		else
		{
			it++;
			it.get()->prev = to_delete->prev;
			to_delete->prev->next = it.get();
			delete to_delete;
			size--;
		}
	}
	return deleted;
}

template <class T>
bool List<T>::insert(T object, Iterator<T> &it)
{
	Node<T>* new_node = new Node<T>(object);
	if (it.get() == first)
	{
		new_node->prev = nullptr;
		new_node->next = first;
		first->prev = new_node;
		first = new_node;
		it.set(first);
	}
	else
	{
		if (it.get() == last)
		{
			new_node->next = nullptr;
			new_node->prev = last;
			last->next = new_node;
			last = new_node;
			it.set(last);
		}
		else
		{
			new_node->next = it.get();
			new_node->prev = it.get()->prev;
			it.get()->prev->next = new_node;
			it.get()->prev = new_node;
		}
	}

	size++;
	return 0;
}

template <class T>
Iterator<T> List<T>::begin()
{
	Iterator<T> it;
	it.set(first);
	return it;
}

template <class T>
Iterator<T> List<T>::end()
{
	Iterator<T> it;
	it.set(last);
	return it;
}
