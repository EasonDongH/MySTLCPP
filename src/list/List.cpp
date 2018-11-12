#include "List.h"
#include <random>
#include <time.h>
#include <iostream>
using namespace std;

template <typename T>
void List<T>::init() {
	this->header = new ListNode<T>;                                   //头哨兵
	this->trailer = new ListNode<T>;                                  //尾哨兵
	this->header->succ = this->trailer; this->header->pred = nullptr;
	this->trailer->succ = nullptr; this->trailer->pred = this->header;
	this->_size = 0;
}

template <typename T>
List<T>::List(List<T> const& L) {
	init();
	ListNodePosi(T) p = L.header->succ;
	while (p != L.trailer) {
		insertAsLast(p->data);
		p = p->succ;
	}
}

template <typename T>
List<T>::List(List<T> const& L, Rank r, int n) {
	init();
	ListNodePosi(T) p = L.header->succ;
	while (p != L.trailer && 0 < r--)
		p = p->succ;
	while (p != nullptr && 0 < n--) {
		insertAsLast(p->data);
		p = p->succ;
	}
}

template <typename T>
List<T>::List(ListNodePosi(T) p, int n) {
	init();
	while (p != nullptr && 0 < n--) {
		insertAsLast(p->data);
		p = p->succ;
	}
}

template <typename T>
List<T>::~List() {
	clear();
	delete this->header;
	delete this->trailer;
}

template <typename T>
int List<T>::clear() {
	int oldSize = this->_size;
	while (0 < this->_size--)
		remove(header->succ);
	return oldSize;
}

template <typename T>
void List<T>::copyNodes(ListNodePosi(T) p, int n) {                   //这里保证p后至少有n-1个结点
	init();
	while (0 < n--) {
		insertAsLast(p->data);
		p = p->succ;
	}
}

template <typename T>
void List<T>::insertionSort(ListNodePosi(T) p, int n) {
	/*ListNodePosi(T) p1;
	for (; p != this->trailer && 0 < n--; p=p->succ) {
		T e = p->data;
		for (p1 = p; p1->pred != this->header; p1 = p1->pred)
			if (e < p1->data)
				p1->data = p1->pred->data;
			else
				break;
		p1->data = e;
	}*/

	for (int r = 0; r < n; r++) {
		insertAsSucc(search(p->data, r, p), p->data);
		p = p->succ;
		remove(p->pred);
	}
}

template <typename T>
void List<T>::selectionSort(ListNodePosi(T) p, int n) {              //保证p后有n个结点
	ListNodePosi(T) trail = p;
	for (int i = 0; i < n; i++)
		trail = trail->succ;
	p = p->pred;
	while (1 < n) {
		ListNodePosi(T) max = selectMax(p->succ, n);                     //selectMax搜索不包含p
		insertAsPred(trail, remove(max));                             //在p后n个结点找到的最大值插入到尾部，并且移除当前最大值结点
		n--; trail = trail->pred;
	}
}

template <typename T>
T& List<T>::operator[](Rank r) const {
	ListNodePosi(T) p = first();
	while (0 < r--) {
		p = p->succ;                                                    //这里保证p后至少有r-1个结点
	}
	return p->data;
}

template <typename T>
bool List<T>::disOrdered() const {                                      //默认升序为有序
	ListNodePosi(T) p = this->header->succ;
	while (p->succ != this->trailer) {
		if (p->succ->data < p->data)
			return true;
		p = p->succ;
	}
	return false;
}

template <typename T>
ListNodePosi(T) List<T>::find(T const& e, int n, ListNodePosi(T) p) const {
	while (0 < n--)                                                    //在p的n个前驱结点中查找e
		if (e == (p = p->pred)->data)                                  //这里保证p后至少有n-1个结点
			return p;
	return nullptr;
}

template <typename T>
ListNodePosi(T) List<T>::search(T const& e, int n, ListNodePosi(T) p) const {
	while (0 <= n--) {
		p = p->pred;
		if (p->data <= e)
			break;
	}
	return p;
}

template <typename T>
ListNodePosi(T) List<T>::selectMax(ListNodePosi(T) p, int n) const {     //区间(p,p+n]
	if (p == nullptr || p == this->trailer)
		return p;
	ListNodePosi(T) theMax = p;
	for (ListNodePosi(T) cur = p; 1 < n; n--) {
		cur = cur->succ;
		if (theMax->data <= cur->data)                                  //相同元素找靠后的
			theMax = cur;
	}

	return theMax;
}

template <typename T>
ListNodePosi(T) List<T>::insertAsFirst(T const& e) {
	this->_size++;
	return this->header->insertAsSucc(e);
}

template <typename T>
ListNodePosi(T) List<T>::insertAsLast(T const& e) {
	this->_size++;
	return this->trailer->insertAsPred(e);
}

template <typename T>
ListNodePosi(T) List<T>::insertAsPred(ListNodePosi(T) p, T const& e) {
	this->_size++;
	return p->insertAsPred(e);
}

template <typename T>
ListNodePosi(T) List<T>::insertAsSucc(ListNodePosi(T) p, T const& e) {
	this->_size++;
	return p->insertAsSucc(e);
}

template <typename T>
T List<T>::remove(ListNodePosi(T) p) {             //保证p的合法性
	T data = p->data;
	p->pred->succ = p->succ;                       //p的前驱的后继改为p的后继
	p->succ->pred = p->pred;                       //p的后驱的前驱改为p的前驱

	delete p;
	this->_size--;
	return data;
}

template <typename T>
void List<T>::sort(ListNodePosi(T) p, int n) {
	//insertionSort(p, n);
	selectionSort(p, n);
}

template <typename T>
int List<T>::deduplicate() {
	/*int oldSize = this->_size;
	for (ListNodePosi(T) p = this->header->succ; p != this->trailer; p = p->succ) {
		for (ListNodePosi(T) q = p->succ; q != this->trailer; ) {
			if (p->data == q->data) {
				ListNodePosi(T) tmp = q->succ;
				remove(q);
				q = tmp;
			}
			else
				q = q->succ;
		}
	}
	return oldSize - this->_size;*/

	if (this->_size < 2) return 0;
	int oldSize = this->_size;
	Rank r = 0;
	ListNodePosi(T) p = this->header->succ;
	while (p != this->trailer) {
		ListNodePosi(T) q = find(p->data, r, p);                //在p的r个前驱中删除与p相同的元素
		q == nullptr ? r++ : remove(q);                         //若存在相同元素，删除之
		p = p->succ;                                            //不存在前驱元素+1
	}
	return oldSize - this->_size;
}

template <typename T>
int List<T>::uniquify() {
	int oldSize = this->_size;
	ListNodePosi(T) p = this->header->succ;
	ListNodePosi(T) q = p->succ;
	while (q != this->trailer) {
		if (p->data != q->data)
			p = q;
		else
			remove(q);

		q = p->succ;
	}

	return oldSize - this->_size;
}

template <typename T>
void visit1(T& e) {
	cout << e << " ";
}

template <typename T>
void List<T>::reverse() {
	if (this->_size == 1)
		return;

	ListNodePosi(T) p1;
	ListNodePosi(T) p2;
	ListNodePosi(T) tmp;

	p1 = this->trailer; p2 = p1->pred;
	while (p1 != this->header) {
		tmp = p2->pred;
		
		p1->succ = p2;
		p2->pred = p1;

		p1 = p2; p2 = tmp;
	}

	tmp = this->header;
	this->header = this->trailer;
	this->trailer = tmp;
}

template <typename T>
void List<T>::traverse(void(*vist)(T&)) {
	for (ListNodePosi(T) p = first(); p != this->trailer; p = p->succ)
		vist(p->data);
}

template<typename T> template<typename VST>
void List<T>::traverse(VST& visit) {
	for (ListNodePosi(T) p = first(); p != this->trailer; p = p->succ)
		vist(p->data);
}