#pragma once
typedef int Rank;                                       //��
#define ListNodePosi(T) ListNode<T>*                    //�б���λ��

template <typename T> struct ListNode {                 //�б���ģ���ࣨ˫������
	T data; ListNodePosi(T) pred; ListNodePosi(T) succ; //��ֵ��ǰ�������

	ListNode(){}
	ListNode(T e, ListNodePosi(T) p=NULL, ListNodePosi(T) s=NULL)
		:data(e),pred(p),succ(s){}

	ListNodePosi(T) insertAsPred(T const& e);            //�ڵ�ǰ���֮ǰ����
	ListNodePosi(T) insertAsSucc(T const& e);            //�ڵ�ǰ���֮�����
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