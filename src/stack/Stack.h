#pragma once
//#include <assert.h>
#include "../vector/vector.h"
#include "../vector/vector.cpp"

template <typename T> class Stack:public Vector<T>
{
public:
	void push(T& e) {                                
		(*this).insert((*this).size(), e);
	}
	T pop() {
		//assert(!(*this).empty());
		return (*this).remove((*this).size() - 1);
	}
	T& top() {
		//assert(!(*this).empty());
		return (*this)[(*this).size() - 1];
	}
};

