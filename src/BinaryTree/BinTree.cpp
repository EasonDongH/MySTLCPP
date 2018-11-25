#include "BinTree.h"

template<typename T>
int BinTree<T>::updateHeight(BinNodePosi(T) x) {
	return x->height = 1 + max(stature(x->lc), stature(x->rc));
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

template <typename T>//二叉树插入：将T当作x的右子树接入，T本身置空
BinNodePosi(T) BinTree<T>::attachAsRC(BinNodePosi(T) x, BinTree<T>* &T) {
	if (x->rc = T->_root) x->rc->parent = x;
	_size += T->_size; updateHeightAbove(x);
	T->_root = nullptr; T->_size = 0; release(T);
	T = nullptr;
	return x;
}

template <typename T>
static int removeAt(BinNodePosi(T) x) {
	if (!x) return 0;
	int n = 1 + removeAt(x->lc) + removeAt(x->rc);
	release(x->data); release(x);
	return n;
}

template <typename T>//删除二叉树中位置x处的节点及其后代，返回被删除节点的数值
int BinTree<T>::remove(BinNodePosi(T) x) {
	FromParentTo(*x) = nullptr;
	updateHeightAbove(x->parent);
	int n = removeAt(x);
	_size -= n;
	return n;
}

template <typename T>
BinTree<T>* BinTree<T>::secde(BinNodePosi(T) x) {
	FromParentTo(*x) = nullptr;//切断来自父节点的指针
	updateHeightAbove(x->parent);
	BinTree<T>* S = new BinTree<T>;
	S->_root = x; x->parent = nullptr;
	S->_size = x->size(); _size -= S->_size;
	return S;
}

template <typename T, typename VST>
void travPre_R(BinNodePosi(T) x, VST& visit) {
	if (!x) return;
	visit(x->data);
	trav_R(x->lc, visit);
	trav_R(x->rc, visit);
}

template <typename T, typename VST>
void travIn_R(BinNodePosi(T) x, VST& visit) {
	if (!x) return;
	trav_R(x->lc, visit);
	visit(x->data);
	trav_R(x->rc, visit);
}

template <typename T, typename VST>
void travPost_R(BinNodePosi(T) x, VST& visit) {
	if (!x) return;
	trav_R(x->lc, visit);
	trav_R(x->rc, visit);
	visit(x->data);
}

#include "../queue/Queue.h"
template <typename T,typename VST>
void trav_Level(BinNodePosi(T) x, VST& visit) {
	Queue<BinNodePosi(T)> Q;
	Q.enqueue(x);
	while (!Q.empty()) {
		x = Q.dequeue();
		if (!x) {
			visit(x->data);
			Q.enqueue(x->lc);
			Q.enqueue(x->rc);
		}
	}
}