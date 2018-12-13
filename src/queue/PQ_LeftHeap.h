#pragma once
#include "PQ.h"
#include "../BinaryTree/BinTree.h"

template <typename T> class PQ_LeftHeap :public PQ<T>, public BinTree<T> {
public:
	PQ_LeftHeap() {}
	PQ_LeftHeap(T* E, int n) {
		for (int i = 0; i < n; i++)
			insert(E[i]);
	}
	void insert(T);
	T getMax();
	T delMax();
};

