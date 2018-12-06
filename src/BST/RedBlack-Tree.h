#pragma once
#include "../BST/BST.h"

template <typename T> class RedBlackTree : public BST<T> {
protected:
	void solveDoubleRed(BinNodePosi(T) x);//˫������
	void solveDoubleBlack(BinNodePosi(T) x);//˫������
	int updateHeight(BinNodePosi(T) x);
public:
	BinNodePosi(T) insert(const T& e);
	bool remove(const T& e);
};

#define IsBack(p) (!(p) || (RB_BLACK == (p)->color))//�ⲿ�ڵ������ڽڵ�
#define IsRed(p) (!IsBack(p))
#define BlackHeightUpdated(x) (/*RedBlack�߶ȸ�������*/\
    (stature((x).lc) == stature((x).rc)) && \
    ((x).height == (IsRed(&x)? stature((x).lc) : stature((x).lc)+1))\
)

template <typename T> 
int RedBlackTree<T>::updateHeight(BinNodePosi(T) x) {
	x->height = max(stature(x->lc), stature(x->rc));//����һ��ڸ߶���ȣ����ǳ���˫��
	return IsBack(x) ? x->height++ : x->height;//����ǰ�ڵ�Ϊ�ڣ������ڸ߶�
}

template <typename T>
BinNodePosi(T) RedBlackTree<T>::insert(const T& e) {
	BinNodePosi(T)& x = search(e);
	if (x)//�Ѵ���
		return x;

	x = new BinNode<T>(e, this->_hot, nullptr, nullptr, -1, RB_RED);//������ڵ�e����_hotΪ�����ڸ߶�Ϊ-1
	solveDoubleRed(x);
	return (x) ? x : _hot->parent;
}

/*
 *   RedBlack˫������㷨������ڵ�x���丸��Ϊ��ɫ�����⣬��Ϊ�����������
            RR-1��2����ɫ��ת��2�κڸ߶ȸ��£�1-2����ת�����ٵݹ�
			RR-2��3����ɫ��ת��3�κڸ߶ȸ��£�0����ת����Ҫ�ݹ�
 */
template <typename T>
void RedBlackTree<T>::solveDoubleRed(BinNodePosi(T) x) {
	if (IsRoot(*x)) {//������������Ⱦ�ڣ������ڸ߶���֮����
		_root->color = RB_BLACK;
		_root->height++;
		return;
	}

	BinNodePosi(T) p = x->parent;//����x�ĸ��ױش���
	if (IsBack(p)) return;//��pΪ�ڣ���ֹ����
	BinNodePosi(T) g = p->parent;//��ȻpΪ�죬g��Ȼ������Ϊ��
	BinNodePosi(T) u = uncle(x);
	if (IsBack(u)) {//3+4�ع�
		if (IsLChild(*x) == IsLChild(*p))//x��pͬ�࣬zig-zig��zag-zag
			p->color = RB_BLACK;
		else//x��p��࣬zig-zag��zag-zig
			x->color = RB_BLACK;
		g->color = RB_RED;

		BinNodePosi(T) gg = g->parent;
		BinNodePosi(T) r = FromParentTo(*g) = rotatAt(x);
		r->parent = gg;
	}
	else {//���ƴ�������
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
 *    RedBlack˫�ڵ����㷨������ڵ�x�뱻�����Ľڵ��Ϊ��ɫ������
 *    ��Ϊ�����๲���������
 *           BB-1��2����ɫ��ת��2�κڸ߶ȸ��£�1-2����ת�����ٵݹ�
 *           BB-2R��2����ɫ��ת��2�κڸ߶ȸ��£�0����ת�����ٵݹ�
 *           BB-2B��1����ɫ��ת��1�κڸ߶ȸ��£�0����ת����Ҫ�ݹ�
 *           BB-3��2����ɫ��ת��2�κڸ߶ȸ��£�1����ת��תΪBB-1��BB2R
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