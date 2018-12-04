#pragma once

#include "BST.h"

template <typename T> class Splay :public BST<T> {
protected:
	BinNodePosi(T) splay(BinNodePosi(T) v);//将节点伸展至树根
public:
	BinNodePosi(T) search(const T& e);
	BinNodePosi(T) insert(const T& e);
	bool remove(const T& e);
};


template <typename NodePosi> inline
void attachLChild(NodePosi p, NodePosi lc) {
	p->lc = lc;
	if (lc)
		lc->parent = p;
}

template <typename NodePosi> inline
void attachRChild(NodePosi p, NodePosi rc) {
	p->rc = rc;
	if (rc)
		rc->parent = p;
}

template <typename T>
BinNodePosi(T) Splay<T>::splay(BinNodePosi(T) v) {
	if (!v) return nullptr;
	BinNodePosi(T) p; BinNodePosi(T) g;
	while ((p = v->parent) && (g = p->parent)) {
		BinNodePosi(T) gg = g->parent;
		if (IsLChild(*v)) {
			if (IsLChild(*p)) {//zig-zig
				attachRChild(v, p);
				attachLChild(p, v->rc); attachRChild(p, g);
				attachLChild(g, p->rc);
			}
			else {//zig-zag
				attachLChild(v, g); attachRChild(v, p);
				attachRChild(g, v->lc);
				attachLChild(p, v->rc);
			}
		}
		else {
			if (IsLChild(*p)) {//zag-zig
				attachLChild(v, p); attachRChild(v, g);
				attachRChild(p, v->lc);
				attachLChild(g, v->rc);
			}
			else {//zag-zag
				attachLChild(v, p);
				attachLChild(p, g); attachRChild(p, v->lc);
				attachRChild(g, p->lc);
			}
		}
		if (!gg) v->parent = nullptr;//若g的父节点不存在，则v现在为树根
		else {//否则，v代替g作为gg的左/右孩子
			(g == gg->lc) ? attachLChild(gg, v) : attachRChild(gg, v);
		}
		updateHeight(g); updateHeight(p); updateHeight(v);
	}//双层伸展结束，g必为null，p可能为null
	if (p = v->parent) {//p非空，树有奇数层，还需要一次单旋
		if (IsLChild(*v)) {
			attachLChild(p, v->rc); attachRChild(v, p);
		}
		else {
			attachLChild(v, p); attachRChild(p, v->lc);
		}
		updateHeight(p); updateHeight(v);
	}
	v->parent = nullptr;
	return v;
}

template <typename T>
BinNodePosi(T) Splay<T>::search(const T& e) {
	BinNodePosi(T) p = searchIn(_root, e, _hot = nullptr);
	splay(p ? p : _hot);//将查找的节点伸展到树根；若不存在，则将最后抵达的节点伸展至树根
	return _root;
}

template <typename T>
BinNodePosi(T) Splay<T>::insert(const T& e) {
	if (_root == nullptr) {//树空
		++_size;
		return _root = new BinNode<T>(e);
	}

	if (e == search(e)->data)//已存在，且通过查找，节点e伸展至树根
		return _root;

	++_size;
	BinNodePosi(T) t = _root;
	if (_root->data < e) {//插入新根，以t与t->rc为左右孩子
		t->parent = _root = new BinNode<T>(e, nullptr, t, t->rc);
		if (HasRChild(*t)) {
			t->rc->parent = _root;
			t->rc = nullptr;
		}
	}
	else {//插入新根，以t->lc与t作为左右孩子
		t->parent = _root = new BinNode<T>(e, nullptr, t->rc, t);
		if (HasLChild(*t)) {
			t->lc->parent = _root;
			t->lc = nullptr;
		}
	}

	updateHeightAbove(t);
	return _root;
}

template <typename T>
bool Splay<T>::remove(const T& e) {
	if (_root == nullptr || e != search(e)->data)//树空或元素不存在
		return false;

	BinNodePosi(T) old_Root = _root;
	if (!HasLChild(*_root)) {//无左孩子，直接删除
		_root = _root->rc;
		if (_root)
			_root->parent = nullptr;
	}
	else if (!HasRChild(*_root)) {//无右孩子，直接删除
		_root = _root->lc;
		if (_root)
			_root->parent = nullptr;
	}
	else {//同时有左右孩子，用右子树的最小节点做新树根
		BinNodePosi(T) left_Tree = _root->lc;
		left_Tree->parent = nullptr; _root->lc = nullptr; //左子树被切断
		_root = _root->rc; _root->parent = nullptr;//_root转到右子树
		search(old_Root->data);//以原树根为目标，做一次查找（必然失败）
		//但至此，右子树中最小节点被伸展至树根,且该节点左子树必然为空
		_root->lc = left_Tree; left_Tree->parent = _root;//原左子树接上
	}
	release(old_Root->data); release(old_Root);
	--_size;
	if (_root)
		updateHeight(_root);

	return true;
}