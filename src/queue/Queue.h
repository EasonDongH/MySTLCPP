#pragma once
#include "../list/List.h"

template <typename T> class Queue:public List<T>{
public:
	void enqueue(const T& e) {
		(*this).insertAsLast(e);
	}
	T dequeue() {
		return (*this).remove((*this).first());
	}
	T& front() {
		return ((*this).first())->data;
	}
};
