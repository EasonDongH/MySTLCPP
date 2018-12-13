#pragma once
#include "PQ.h"
#include "../vector/vector.h"

#define InHeap(n,i)                  (((-1) < (i)) && ((i) < n)))
#define Parent(i)                      ((i-1) >> 1)
#define LastInternal(n)            Parent(n-1)//末节点父亲
#define LChild(i)                      (1 + ((i) << 1))
#define RChild(i)                      ((1 + (i)) << 1)
#define ParentValid(i)              (0 < (i))//i是否有父节点
#define LChildValid(n,i)            InHeap(n, LChild(i))
#define RChildValid(n,i)            InHeap(n, RChild(i))
#define Bigger(PQ, i, j)             (It(PQ[i], PQ[j])? j : i)
#define ProperParent(PQ, n, i)/*父、左、右三节点最大者，相等时父节点优先*/  \
                   (RChildValid(n, i) ? Bigger(PQ, Bigger(PQ, i, LChild(i)), RChild(i)) :\
                   (LChildValid(n, i) ? Bigger(PQ, i, LChild(i)) : i ) )

template <typename T> class PQ_ComplHeap :public PQ<T>, public Vector<T> {
protected:
	Rank percolateDown(Rank n, Rank i);//下滤
	Rank percolateUp(Rank i);//上滤
	void heapify(Rank n);//Floyd建堆
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

