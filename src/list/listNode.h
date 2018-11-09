#pragma once
typedef int Rank;                                       //秩
#define ListNodePosi(T) ListNode<T>*                    //列表结点位置

template <typename T> struct ListNode {                 //列表结点模板类（双向链表）
	T data; ListNodePosi(T) pred; ListNodePosi(T) succ; //数值、前驱、后继

	ListNode(){}
	ListNode(T e, ListNodePosi(T) p=NULL, ListNodePosi(T) s=NULL)
		:data(e),pred(p),succ(s){}

	ListNodePosi(T) insertAsPred(T const& e);            //在当前结点之前插入
	ListNodePosi(T) insertAsSucc(T const& e);            //在当前结点之后插入
};

template <typename T>
ListNodePosi(T) ListNode<T>::insertAsPred(T const& e) {
	ListNodePosi(T) p = new ListNode(e, this->pred, this);
	this->pred->succ = p;
	this->pred = p;
	return p;
}

template <typename T>
ListNodePosi(T) ListNode<T>::insertAsSucc(T const& e) {
	ListNodePosi(T) p = new ListNode(e, this, this->succ);
	this->succ->pred = p;
	this->succ = p;
	return p;
}