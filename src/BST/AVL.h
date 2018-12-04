#pragma once

#include "BST.h"

#define Balanced(x) (stature((x).lc) == stature((x).rc))//����ƽ��
#define BalFac(x) (stature((x).lc) - stature((x).rc))//ƽ������
#define AvlBalanced ((-2 < BalFac(x)) && (BalFac(x) < 2))//AVLƽ������
#define tallerChild(x)(\
        stature((x)->lc) > stature((x)->rc) ? (x)->lc : (\
/*���*/stature((x)->lc) < stature((x)->rc) ? (x)->rc : (\
/*�Ҹ�*/IsLChild(*(x)) ? (x)->lc : (x)->rc /*�ȸߣ��븸��xͬ��������*/\
        )\
        )\
)

template <typename T> class AVL :public BST<T> {
public:
	BinNodePosi(T) insert(const T& e);
	bool remove(const T& e);
};

template <typename T>
BinNodePosi(T) AVL<T>::insert(const T& e) {
	BinNodePosi(T)& x = search(e);
	if (x) return x;//�Ѵ���
	//�½ڵ�
	BinNodePosi(T) xx = x = new BinNode<T>(e, this->_hot);
	++this->_size();
	//x�ĸ������ߣ����游����ʧ��
	for (BinNodePosi(T) g = this->_hot; g; g = g->parent) {//������������
		if (!AvlBalanced(*g)) {//һ��ʧ�⣬�򣨲���3+4�㷨��ʹ֮����
			FromParentTo(*g) = rotatAt(tallerChild(tallerChild(g)));//���½���ԭ��
			break;//�ֲ������߶ȸ�ԭ������������ˣ��ʵ�������
		}
		else
			updateHeight(g);
	}//������Ҫһ�ε���
	return xx;
}

template <typename T>
bool AVL<T>::remove(const T& e) {
	BinNodePosi(T)& x = search(e);
	if (!x) return false;//������
	removeAt(x, this->_hot); --this->_size;
	for (BinNodePosi(T) g = this->_hot; g; g = g->parent) {
		if (!AvlBalanced(*g)) {
			g = FromParentTo(*g) = rotatAt(tallerChild(tallerChild(g)));
		}
		updateHeight(g);
	}
	return true;
}