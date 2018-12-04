#pragma once

#include "BST.h"

template <typename T> class Splay :public BST<T> {
protected:
	BinNodePosi(T) splay(BinNodePosi(T) v);//���ڵ���չ������
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
		if (!gg) v->parent = nullptr;//��g�ĸ��ڵ㲻���ڣ���v����Ϊ����
		else {//����v����g��Ϊgg����/�Һ���
			(g == gg->lc) ? attachLChild(gg, v) : attachRChild(gg, v);
		}
		updateHeight(g); updateHeight(p); updateHeight(v);
	}//˫����չ������g��Ϊnull��p����Ϊnull
	if (p = v->parent) {//p�ǿգ����������㣬����Ҫһ�ε���
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
	splay(p ? p : _hot);//�����ҵĽڵ���չ���������������ڣ������ִ�Ľڵ���չ������
	return _root;
}

template <typename T>
BinNodePosi(T) Splay<T>::insert(const T& e) {
	if (_root == nullptr) {//����
		++_size;
		return _root = new BinNode<T>(e);
	}

	if (e == search(e)->data)//�Ѵ��ڣ���ͨ�����ң��ڵ�e��չ������
		return _root;

	++_size;
	BinNodePosi(T) t = _root;
	if (_root->data < e) {//�����¸�����t��t->rcΪ���Һ���
		t->parent = _root = new BinNode<T>(e, nullptr, t, t->rc);
		if (HasRChild(*t)) {
			t->rc->parent = _root;
			t->rc = nullptr;
		}
	}
	else {//�����¸�����t->lc��t��Ϊ���Һ���
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
	if (_root == nullptr || e != search(e)->data)//���ջ�Ԫ�ز�����
		return false;

	BinNodePosi(T) old_Root = _root;
	if (!HasLChild(*_root)) {//�����ӣ�ֱ��ɾ��
		_root = _root->rc;
		if (_root)
			_root->parent = nullptr;
	}
	else if (!HasRChild(*_root)) {//���Һ��ӣ�ֱ��ɾ��
		_root = _root->lc;
		if (_root)
			_root->parent = nullptr;
	}
	else {//ͬʱ�����Һ��ӣ�������������С�ڵ���������
		BinNodePosi(T) left_Tree = _root->lc;
		left_Tree->parent = nullptr; _root->lc = nullptr; //���������ж�
		_root = _root->rc; _root->parent = nullptr;//_rootת��������
		search(old_Root->data);//��ԭ����ΪĿ�꣬��һ�β��ң���Ȼʧ�ܣ�
		//�����ˣ�����������С�ڵ㱻��չ������,�Ҹýڵ���������ȻΪ��
		_root->lc = left_Tree; left_Tree->parent = _root;//ԭ����������
	}
	release(old_Root->data); release(old_Root);
	--_size;
	if (_root)
		updateHeight(_root);

	return true;
}