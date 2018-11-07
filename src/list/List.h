#pragma once
#include "listNode.h"

template <typename T> class List
{
private:
	int _size;                                                    //规模
	ListNodePosi(T) header; ListNodePosi(T) trailer;              //头、尾哨兵
protected:
	void init();                                                  //创建列表时初始化
	int clear();                                                  //清除当前所有结点
	void copyNodes(ListNodePosi(T) p, int n);                     //复制自结点p开始的n的结点
	void merge(ListNodePosi(T)&, int, List<T>&, ListNodePosi(T), int);
	void mergeSort(ListNodePosi(T)&, int);

public:
	List() { init(); }
	List(List<T> const& L);                                        //整体复制列表
	List(List<T> const& L, Rank r, int n);                         //复制L中自r起的n项
	List(ListNodePosi(T) p, int n);                                //复制自p起的n项

	~List();

	Rank size() const { return this->_size; }
	bool empty() const { return this->size <= 0; }
	T& operator[](Rank r) const;
	ListNodePosi(T) first() const { return this->header->succ; }   //首节点
	ListNodePosi(T) last() const { return this->trailer->pred; }   //尾节点
	bool valid(ListNodePosi(T) p) {                                //判断p对外是否合法
		return p && p != this->header&&p != this->trailer;
	}
	int disOrdered() const;
	ListNodePosi(T) find(T const& e) const {                       //无序查找
		return find(e, _size, this->trailer);
	}
	ListNodePosi(T) find(T const& e,int n,ListNodePosi(T) p) const;//无序区间查找
	ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const;//有序区间查找
	ListNodePosi(T) search(T const& e) {
		return search(e, this->_size, this->trailer);
	}
	ListNodePosi(T) selectMax(ListNodePosi(T) p, int n) const;         //在p之后的n个结点查找最大值
	ListNodePosi(T) selecrMax()const {
		return selectMax(this->header->succ, this->_size);
	}

	ListNodePosi(T) insertAsFirst(T const& e);
	ListNodePosi(T) insertAsLast(T const& e);
	ListNodePosi(T) insertAsSucc(T const& e);
	ListNodePosi(T) insertAsPred(T const& e);
	T remove(ListNodePosi(T) p);
	void merge(List<T>& L) {
		merge(first(), this->_size, L, L, first(), L._size);
	}
	void sort(ListNodePosi(T) p, int n);
	void sort() {
		sort(this->header->succ, this->_size);
	}
	int deduplicate();                                           //无序去重
	int uniquify();                                              //有序去重
	void reverse();

	void traverse(void(*)(T&));
	template <typename VST> void traverse(VST&);
};