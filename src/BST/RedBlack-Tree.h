#pragma once
#include "../BST/BST.h"

template <typename T> class RedBlackTree : public BST<T> {
protected:
	void solveDoubleRed(BinNodePosi(T) x);//双红修正
	void solveDoubleBlack(BinNodePosi(T) x);//双黑修正
	int updateHeight(BinNodePosi(T) x);
public:
	BinNodePosi(T) insert(const T& e);
	bool remove(const T& e);
};

#define IsBack(p) (!(p) || (RB_BLACK == (p)->color))//外部节点视作黑节点
#define IsRed(p) (!IsBack(p))
#define BlackHeightUpdated(x) (/*RedBlack高度更新条件*/\
    (stature((x).lc) == stature((x).rc)) && \
    ((x).height == (IsRed(&x)? stature((x).lc) : stature((x).lc)+1))\
)

template <typename T> 
int RedBlackTree<T>::updateHeight(BinNodePosi(T) x) {
	x->height = max(stature(x->lc), stature(x->rc));//孩子一般黑高度相等，除非出现双黑
	return IsBack(x) ? x->height++ : x->height;//若当前节点为黑，则计入黑高度
}

template <typename T>
BinNodePosi(T) RedBlackTree<T>::insert(const T& e) {
	BinNodePosi(T)& x = search(e);
	if (x)//已存在
		return x;

	x = new BinNode<T>(e, this->_hot, nullptr, nullptr, -1, RB_RED);//创建红节点e，以_hot为父，黑高度为-1
	solveDoubleRed(x);
	return (x) ? x : _hot->parent;
}

/*
 *   RedBlack双红调整算法：解决节点x与其父均为红色的问题，分为两大类情况：
            RR-1：2次颜色翻转，2次黑高度更新，1-2次旋转，不再递归
			RR-2：3次颜色翻转，3次黑高度更新，0次旋转，需要递归
 */
template <typename T>
void RedBlackTree<T>::solveDoubleRed(BinNodePosi(T) x) {
	if (IsRoot(*x)) {//至树根：将其染黑，整树黑高度随之递增
		_root->color = RB_BLACK;
		_root->height++;
		return;
	}

	BinNodePosi(T) p = x->parent;//否则x的父亲必存在
	if (IsBack(p)) return;//若p为黑，终止调整
	BinNodePosi(T) g = p->parent;//既然p为红，g必然存在且为黑
	BinNodePosi(T) u = uncle(x);
	if (IsBack(u)) {//3+4重构
		if (IsLChild(*x) == IsLChild(*p))//x与p同侧，zig-zig或zag-zag
			p->color = RB_BLACK;
		else//x与p异侧，zig-zag或zag-zig
			x->color = RB_BLACK;
		g->color = RB_RED;

		BinNodePosi(T) gg = g->parent;
		BinNodePosi(T) r = FromParentTo(*g) = rotatAt(x);
		r->parent = gg;
	}
	else {//类似处理上溢
		p->color = RB_BLACK; ++p->height;
		u->color = RB_BLACK; ++u->height;
		if (!IsRoot(*g))
			g->color = RB_RED;
		solveDoubleRed(g);
	}
}

template <typename T>
bool RedBlackTree<T>::remove(const T& e) {
	BinNodePosi(T) & x = search(e);
	if (!x)
		return false;

	BinNodePosi(T) r = removeAt(x, _hot);
	if (!(--_size)) return true;
	if (!_hot) {
		_root->color = RB_BLACK;
		updateHeight(_root);
		return true;
	}
	if (BlackHeightUpdated(*_hot))
		return true;
	if (IsRed(r)) {
		r->color = RB_BLACK;
		++r->height;
		return true;
	}
	solveDoubleBlack(r);
	return true;
}

/*
 *    RedBlack双黑调整算法：解决节点x与被其代替的节点均为黑色的问题
 *    分为三大类共四种情况：
 *           BB-1：2次颜色翻转，2次黑高度更新，1-2次旋转，不再递归
 *           BB-2R：2次颜色翻转，2次黑高度更新，0次旋转，不再递归
 *           BB-2B：1次颜色翻转，1次黑高度更新，0次旋转，需要递归
 *           BB-3：2次颜色翻转，2次黑高度更新，1次旋转，转为BB-1或BB2R
 */
template <typename T>
void RedBlackTree<T>::solveDoubleBlack(BinNodePosi(T) r) {
	BinNodePosi(T) p = r ? r->parent : _hot;
	if (!p)
		return;
	BinNodePosi(T) s = (r == p->lc) ? p->rc : p->lc;
	if (IsBack(s)) {
		BinNodePosi(T) t = nullptr;
		if (IsRed(s->rc)) t = s->rc;
		if (IsRed(s->lc)) t = s->lc;
		if (t) {
			RBColor oldColor = p->color;
			BinNodePosi(T) b = FromParentTo(*p) = rotatAt(t);
			if (HasLChild(*b)) {
				b->lc->color = RB_BLACK;
				updateHeight(b->lc);
			}
			if (HasRChild(*b)) {
				b->rc->color = RB_BLACK;
				updateHeight(b->rc);
			}
			b->color = oldColor;
			updateHeight(b);
		}
		else {
			s->color = RB_RED;
			--s->height;
			if (IsRed(p)) {
				p->color = RB_BLACK;
			}
			else {
				--p->height;
				solveDoubleBlack(p);
			}
		}
	}
	else {
		s->color = RB_BLACK;
		p->color = RB_RED;
		BinNodePosi(T) t = IsLChild(*s) ? s->lc : s->rc;
		_hot = p;
		FromParentTo(*p) = rotatAt(t);
		solveDoubleBlack(r);
	}
}