#pragma once
#include "listNode.h"

template <typename T> class List
{
private:
	int _size;                                                    //��ģ
	ListNodePosi(T) header; ListNodePosi(T) trailer;              //ͷ��β�ڱ�
protected:
	void init();                                                  //�����б�ʱ��ʼ��
	int clear();                                                  //�����ǰ���н��
	void copyNodes(ListNodePosi(T) p, int n);                     //�����Խ��p��ʼ��n�Ľ��
	void merge(ListNodePosi(T)&, int, List<T>&, ListNodePosi(T), int);
	void mergeSort(ListNodePosi(T)&, int);

public:
	List() { init(); }
	List(List<T> const& L);                                        //���帴���б�
	List(List<T> const& L, Rank r, int n);                         //����L����r���n��
	List(ListNodePosi(T) p, int n);                                //������p���n��

	~List();

	Rank size() const { return this->_size; }
	bool empty() const { return this->size <= 0; }
	T& operator[](Rank r) const;
	ListNodePosi(T) first() const { return this->header->succ; }   //�׽ڵ�
	ListNodePosi(T) last() const { return this->trailer->pred; }   //β�ڵ�
	bool valid(ListNodePosi(T) p) {                                //�ж�p�����Ƿ�Ϸ�
		return p && p != this->header&&p != this->trailer;
	}
	int disOrdered() const;
	ListNodePosi(T) find(T const& e) const {                       //�������
		return find(e, _size, this->trailer);
	}
	ListNodePosi(T) find(T const& e,int n,ListNodePosi(T) p) const;//�����������
	ListNodePosi(T) search(T const& e, int n, ListNodePosi(T) p) const;//�����������
	ListNodePosi(T) search(T const& e) {
		return search(e, this->_size, this->trailer);
	}
	ListNodePosi(T) selectMax(ListNodePosi(T) p, int n) const;         //��p֮���n�����������ֵ
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
	int deduplicate();                                           //����ȥ��
	int uniquify();                                              //����ȥ��
	void reverse();

	void traverse(void(*)(T&));
	template <typename VST> void traverse(VST&);
};