#pragma once
#include "../vector/vector.h"
#define BTNodePosi(T) BTNode<T>*

template <typename T> struct BTNode {
	BTNodePosi(T) parent;
	Vector<BTNodePosi(T)> key;
	Vector<BTNodePosi(T)> child;
	//初始时有0个关键码和1个空孩子指针
	BTNode() {
		parent = nullptr;
		child.insert(0, nullptr);
	}

	BTNode(T e, BTNodePosi(T) lc = nullptr, BTNodePosi(T) rc = nullptr) {
		parent = nullptr;
		key.insert(0, e);
		child.insert(0, lc); child.insert(1, rc);
		if (lc)
			lc->parent = this;
		if (rc)
			rc->parent = this;
	}
};

template <typename T> class BTree {
protected:
	int _size;
	int _order;//B树的阶次，至少为3
	BTNodePosi(T) _root;
	BTNodePosi(T) _hot;
	void solveOverflow(BTNodePosi(T));//因插入而上溢后的分裂处理
	void solveUnderflow(BTNodePosi(T));//因删除而下溢后的合并处理
public:
	BTree(int order = 3) :_order(order), _size(0) {
		_root = new BTNode<T>();
	}

	BTree() {
		if (_root)
			release(_root);//释放所有节点
	}

	const int order() {
		return this->_order;
	}

	const int size() {
		return this->_size;
	}

	BTNodePosi(T)& root() {
		return this->_root;
	}

	bool empty() const {
		return !this->_root;
	}

	BTNodePosi(T) search(const T& e);
	bool insert(const T& e);
	bool remove(const T& e);
	void release(BTNodePosi(T) v) {
		BTNodePosi(T)  p;
		while (v) {
			p = v->parent;
			for (int i = 0; i < v->child.size(); ++i)
				if (v->child[i])
					delete v->child[i];
			v = p;
			--this->_size;
		}
		this->_root = this->_hot = nullptr;
	}
};

template <typename T>
BTNodePosi(T) BTree<T>::search(const T& e) {
	BTNodePosi(T) v = this->_root;
	this->_hot = nullptr;
	while (v) {
		Rank r = v->key.search(e);//在当前节点寻找不小于e的最大关键码
		if (0 <= r && v->key[r] == e)
			return v;
		this->_hot = v;
		v = v->child[r + 1];//转入子树（IO操作）
	}
	return nullptr;
}

template <typename T>
bool BTree<T>::insert(const T& e) {
	BTNodePosi(T) v = search(e);
	if (v)//已存在
		return false;
	Rank r = this->_hot->key.search(e);//查找合适的插入位置
	this->_hot->key.insert(r + 1, e);
	this->_hot->child.insert(r + 2, nullptr);//因为此时必然是在外部节点（全是空节点）插入空节点，等效于在最后直接插入
	++this->_size;
	solveOverflow(this->_hot);
	return true;
}

template <typename T>
void BTree<T>::solveOverflow(BTNodePosi(T) v) {
	if (v->child.size() <= _order) //递归基
		return;

	Rank s = _order / 2;//此时有_order == key.size() == child.size()+1
	BTNodePosi(T) u = new BTNode<T>();
	for (Rank j = 0; j < _order - s - 1; ++j) {//将v右半节点转移至u
		u->child.insert(j, v->child.remove(s + 1));
		u->key.insert(j, v->key.remove(s + 1));
	}
	u->child[_order - s - 1] = v->child.remove(s + 1);//最右侧孩子（只移动孩子，不移动关键码）
	for (Rank j = 0; j < _order - s && u->child[j]; ++j)
		u->child[j]->parent = u;
	BTNodePosi(T) p = v->parent;
	if (!p) {//已至根节点
		_root = p = new BTNode<T>;
		p->child[0] = v;
		v->parent = p;
	}
	Rank r = 1 + p->key.search(v->key[0]);
	p->key.insert(r, v->key.remove(s));
	p->child.insert(r + 1, u);
	u->parent = p;

	solveOverflow(p);//至多递归logn层（树高）
}

template <typename T>
bool BTree<T>::remove(const T& e) {
	BTNodePosi(T) v = search(e);
	if (!v)//节点不存在
		return false;

	Rank r = v->key.search(e);
	if (v->child[0]) {
		BTNodePosi(T) u = v->child[r + 1];
		while (u->child[0])//在右子树中一直向左
			u = u->child[0];//找到e的后继
		v->key[r] = u->key[0];//并与之交换位置
		v = u; r = 0;
	}//至此，v必然位于最底层，且其中第r个关键码就是待删除者
	v->key.remove(r);
	v->child.remove(r + 1);//同理，所有孩子都是空，因此可以等效为直接删除最后一个
	--this->_size;
	solveUnderflow(v);

	return true;
}

template <typename T>
void BTree<T>::solveUnderflow(BTNodePosi(T) v) {
	if ((this->_order + 1) / 2 <= v->child.size())//递归基
		return;

	BTNodePosi(T) p = v->parent;
	if (!p) {//已到根节点
		if (0 == v->key.size() && v->child[0]) {//树根v已不包含关键码，却又有唯一的非空孩子
			this->_root = v->child[0];
			this->_root->parent = nullptr;
			v->child[0] = nullptr;
			release(v);
		}
		return;
	}

	Rank r = 0;
	while (p->child[r] != v)
		++r;//确定v是p的第r个孩子

	//情况1：向左兄弟借关键码
	if (0 < r) {
		BTNodePosi(T) ls = p->child[r - 1];//左兄弟
		if ((_order + 1) / 2 < ls->child.size()) {//该兄弟有足够多的孩子，可以借出1个
			v->key.insert(0, p->key[r - 1]);//p借出一个关键码给v（作为最小关键码）
			p->key[r - 1] = ls->key.remove(ls->key.size() - 1);//ls的最大关键码转入p
			v->child.insert(0, ls->child.remove(ls->child.size() - 1));//ls的最右侧孩子过继给v

			if (v->child[0])
				v->child[0]->parent = v;
			return;
		}
	}
	//情况2：向有兄弟借关键码
	if (r < p->child.size() - 1) {
		BTNodePosi(T) rs = p->child[r + 1];//右兄弟
		if ((_order + 1) / 2 < rs->child.size()) {//右兄弟有足够多的节点
			v->key.insert(v->key.size(), p->key[r]);//p借出一个关键码给v（作为最大关键码）
			p->key[r] = rs->key.remove(0);//rs的最小关键码转入p
			v->child.insert(v->child.size(), rs->child.remove(0));//将rs的最左孩子过继给v

			if (v->child[v->child.size() - 1])
				v->child[v->child.size() - 1]->parent = v;
			return;
		}
	}
	//情况3：左、右兄弟要么为空，要么都太瘦
	if (0 < r) {//与左兄弟合并
		BTNodePosi(T) ls = p->child[r - 1];
		//p的第r-1个关键码转入ls，v不再是p的第r个孩子
		ls->key.insert(ls->key.size(), p->key.remove(r - 1));
		p->child.remove(r);
		ls->child.insert(ls->child.size(), v->child.remove(0));
		//v的最左侧孩子过继给ls做最右侧孩子
		if (ls->child[ls->child.size() - 1])
			ls->child[ls->child.size() - 1]->parent = ls;
		//v剩余关键码和孩子一次转入ls
		while (!v->key.empty()) {
			ls->key.insert(ls->key.size(), v->key.remove(0));
			ls->child.insert(ls->child.size(), v->child.remove(0));
			if (ls->child[ls->child.size() - 1])
				ls->child[ls->child.size() - 1]->parent = ls;
		}
		release(v);
	}
	else {//右兄弟合并
		BTNodePosi(T) rs = p->child[r + 1];
		rs->key.insert(0, p->key.remove(r));
		p->child.remove(r);
		rs->child.insert(0, v->child.remove(v->child.size() - 1));
		if (rs->child[0])
			rs->child[0]->parent = rs;
		while (!v->key.empty()) {
			rs->key.insert(0, v->key.remove(v->key.size() - 1));
			rs->child.insert(0, v->child.remove(v->child.size() - 1));
			if (rs->child[0])
				rs->child[0]->parent = rs;
		}
		release(v);
	}
	solveUnderflow(p);
	return;
}