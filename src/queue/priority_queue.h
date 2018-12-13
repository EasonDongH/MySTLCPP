#pragma once
#include "PQ.h"
#include "../vector/vector.h"

#define InHeap(n,i)                  (((-1) < (i)) && ((i) < n)))
#define Parent(i)                      ((i-1) >> 1)
#define LastInternal(n)            Parent(n-1)//ĩ�ڵ㸸��
#define LChild(i)                      (1 + ((i) << 1))
#define RChild(i)                      ((1 + (i)) << 1)
#define ParentValid(i)              (0 < (i))//i�Ƿ��и��ڵ�
#define LChildValid(n,i)            InHeap(n, LChild(i))
#define RChildValid(n,i)            InHeap(n, RChild(i))
#define Bigger(PQ, i, j)             (It(PQ[i], PQ[j])? j : i)
#define ProperParent(PQ, n, i)/*�����������ڵ�����ߣ����ʱ���ڵ�����*/  \
                   (RChildValid(n, i) ? Bigger(PQ, Bigger(PQ, i, LChild(i)), RChild(i)) :\
                   (LChildValid(n, i) ? Bigger(PQ, i, LChild(i)) : i ) )

template <typename T> class PQ_ComplHeap :public PQ<T>, public Vector<T> {
protected:
	Rank percolateDown(Rank n, Rank i);//����
	Rank percolateUp(Rank i);//����
	void heapify(Rank n);//Floyd����
public:
	PQ_ComplHeap() {}
	PQ_ComplHeap(T* A, Rank n) {
		copyFrom(A, 0, n);
		heapify(n);
	}
	void insert(T);
	T getMax();
	T delMax();
};

