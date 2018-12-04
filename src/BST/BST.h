#pragma once

#include "../BinaryTree/BinTree.h"

template <typename T> class BST :public BinTree<T> {
protected:
	BinNodePosi(T) _hot;//�����С��ڵ�ĸ���
	BinNodePosi(T) connect34(//���ա�3+4���ṹ������3���ڵ㼰�Ŀ�����
		BinNodePosi(T), BinNodePosi(T), BinNodePosi(T),
		BinNodePosi(T), BinNodePosi(T), BinNodePosi(T), BinNodePosi(T));
	BinNodePosi(T) rotatAt(BinNodePosi(T) x);//��x���丸�ס��游��ͬ����ת����
public:
	virtual BinNodePosi(T)& search(const T& e);
	virtual BinNodePosi(T) insert(const T& e);
	virtual bool remove(const T& e);
};

template<typename T>
static BinNodePosi(T)& searchIn(BinNodePosi(T)& v, const T& e, BinNodePosi(T)& hot) {
	if (!v || e == v->data) return v;
	hot = v;//��¼��ǰ�ڵ�
	return searchIn((e < v->data) ? v->lc : v->rc, e, hot);
}//��󷵻�ʱ��ָ����ҽڵ��ͨ���ڱ���hotָ���丸��

template <typename T>
BinNodePosi(T)& BST<T>::search(const T& e) {
	return searchIn(this->_root, e, _hot = nullptr);
}

template <typename T>
BinNodePosi(T) BST<T>::insert(const T& e) {
	BinNodePosi(T) x = search(e);
	if (x) return x;//e�Ѵ���
	x = new BinNode(e, _hot);
	this->_size++;
	this->updateHeightAbove(x);
	return x;
}//����e�Ƿ��Ѵ������������з��ص�x->data == e

template <typename T>
static BinNodePosi(T) removeAt(BinNodePosi(T)& x, BinNodePosi(T)& hot) {
	BinNodePosi(T) w = x;//ʵ�ʱ�ժ���Ľڵ�
	BinNodePosi(T) succ = nullptr;//ʵ�ʱ�ɾ���Ľڵ�Ľ�����
	if (!HasLChild(*x))
		succ = x->rc;
	else if (!HasRChild(*x)) {
		succ = x->lc;
	}
	else {
		w = w->succ();//�����������ҵ�*x��ֱ�Ӻ��*w
		swap(x->data, w->data);
		BinNodePosi(T) u = w->parent;
		((u == w) ? u->rc : u->lc) = succ = w->rc;
	}
	hot = w->parent;//��¼ʵ�ʱ�ɾ���Ľڵ�ĸ���
	if (succ)
		succ->parent = hot;
	release(w->data); release(w); return succ;
}

template <typename T> bool BST<T>::remove(const T& e) {
	BinNodePosi(T)& x = search(e);
	if (!x) return false;
	removeAt(x, this->_hot);
	this->_size--;
	updateHeightAbove(this->_hot);
	return true;
}

template <typename T> BinNodePosi(T) BST<T>::connect34(
	BinNodePosi(T) a, BinNodePosi(T) b, BinNodePosi(T) c,
	BinNodePosi(T) T0, BinNodePosi(T) T1, BinNodePosi(T) T2, BinNodePosi(T) T3
) {
	a->lc = T0; if (T0) T0->parent = a;
	a->rc = T1; if (T1) T1->parent = a;
	updateHeight(a);
	c->lc = T2; if (T2) T2->parent = c;
	c->rc = T3; if (T3) T3->parent = c;
	updateHeight(c);
	b->lc = a; a->parent = b;
	b->rc = c; c->parent = b;
	updateHeight(b);
	return b;
}

template <typename T> BinNodePosi(T) BST<T>::rotatAt(BinNodePosi(T) v) {//vΪ�ǿ��ﱲ�ڵ�
	BinNodePosi(T) p = v->parent; BinNodePosi(T) g = p->parent;
	if (IsLChild(*p)) {//zig
		if (IsLChild(*v)) {//zig-zig
			p->parent = g->parent;
			return connect34(v, p, g, v->lc, v->rc, p->rc, g->rc);
		}
		else {//zig-zag
			v->parent = g->parent;
			return connect34(p, v, g, p->lc, v->lc, v->rc, g->rc);
		}
	}
	else {//zag
		if (IsLChild(*v)) {//zag-zig
			p->parent = g->parent;
			return connect34(g, p, v, g->lc, p->lc, v->lc, v->rc);
		}
		else {//zag-zag
			v->parent = g->parent;
			return connect34(g, v, p, g->lc, v->lc, v->rc, p->rc);
		}
	}
}