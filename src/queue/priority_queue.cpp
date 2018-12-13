#include "priority_queue.h"

template <typename T> T PQ_ComplHeap<T>::getMax() {
	return _elem[0];//���ȼ����
}

template <typename T> void PQ_ComplHeap<T>::insert(T e) {
	Vector<T>::insert(e);//����ĩβ
	percolateUp(_size - 1);
}

template <typename T> Rank PQ_ComplHeap<T>::percolateUp(Rank i) {
	while (ParentValid(i)) {
		Rank p = Parent(i);
		if (It(_elem[i], _elem[p])) break;
		swap(_elem[i], _elem[p]);
		i = p;
	}
	return i;//���˵����ո߶�
}

template <typename T> T PQ_ComplHeap<T>::delMax() {
	T maxItem = _elem[0];
	_elem[0] = _elem[--_size];
	percolateDown(_size, 0);
	return maxItem;
}

template <typename T> Rank PQ_ComplHeap<T>::percolateDown(Rank n, Rank i) {
	Rank bigger;
	while (i != (bigger = ProperParent(_elem, n, i))) {
		swap(_elem[bigger], _elem[i]);
		i = bigger;
	}
	return i;
}

template <typename T> void PQ_ComplHeap<T>::heapify(Rank n) {
	for (int i = LastInternal(n); InHeap(n, i); --i)
		percolateDown(i);
}