#include "BinTree.h"

template<typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x) {
	return x->height = 1 + max(stature(x->lc), stature(x->rc);
}

template<typename T>
void BinTree<T>::updateHeightAbove(BinNodePosi(T) x) {
	while (x) {
		updateHeight(x);
		x = x->parent;
	}
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRoot(const T& e) {
	this->_size = 1;
	return this->_root = new BinNode<T>(e);
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsLC(BinNodePosi(T) x, const T& e) {
	++(this->_size);
	x->insertAsLC(e);
	updateHeightAbove(x);
	return x->lc;
}

template <typename T>
BinNodePosi(T) BinTree<T>::insertAsRC(BinNodePosi(T) x, const T& e) {
	++(this->_size);
	x->insertAsRC(e);
	updateHeightAbove(x);
	return x->rc;
}

template <typename T>
BinNodePosi(T) BinTree<T>::attachAsLC(BinNodePosi(T) x, BinTree<T>* &T) {
	if (T->_root != nullptr) {
		x->lc = T->_root;
		this->_size += T->_size;
		updateHeightAbove(x);
		release(T);
	}

	return x;
}

template <typename T>
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>* &T) {

}