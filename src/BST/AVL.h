#pragma once

#include "BST.h"

#define Balanced(x) (stature((x).lc) == stature((x).rc))//理想平衡
#define BalFac(x) (stature((x).lc) - stature((x).rc))//平衡因子
#define AvlBalanced ((-2 < BalFac(x)) && (BalFac(x) < 2))//AVL平衡条件
#define tallerChild(x)(\
        stature((x)->lc) > stature((x)->rc) ? (x)->lc : (\
/*左高*/stature((x)->lc) < stature((x)->rc) ? (x)->rc : (\
/*右高*/IsLChild(*(x)) ? (x)->lc : (x)->rc /*等高：与父亲x同侧者优先*/\
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
	if (x) return x;//已存在
	//新节点
	BinNodePosi(T) xx = x = new BinNode<T>(e, this->_hot);
	++this->_size();
	//x的父亲增高，其祖父可能失衡
	for (BinNodePosi(T) g = this->_hot; g; g = g->parent) {//逐层检查各代祖先
		if (!AvlBalanced(*g)) {//一旦失衡，则（采用3+4算法）使之复衡
			FromParentTo(*g) = rotatAt(tallerChild(tallerChild(g)));//重新接入原树
			break;//局部子树高度复原，其祖先亦如此，故调整结束
		}
		else
			updateHeight(g);
	}//至多需要一次调整
	return xx;
}

template <typename T>
bool AVL<T>::remove(const T& e) {
	BinNodePosi(T)& x = search(e);
	if (!x) return false;//不存在
	removeAt(x, this->_hot); --this->_size;
	for (BinNodePosi(T) g = this->_hot; g; g = g->parent) {
		if (!AvlBalanced(*g)) {
			g = FromParentTo(*g) = rotatAt(tallerChild(tallerChild(g)));
		}
		updateHeight(g);
	}
	return true;
}