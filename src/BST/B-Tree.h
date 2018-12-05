#pragma once
#include "../vector/vector.h"
#define BTNodePosi(T) BTNode<T>*

template <typename T> struct BTNode {
	BTNodePosi(T) parent;
	Vector<BTNodePosi(T)> key;
	Vector<BTNodePosi(T)> child;
	//��ʼʱ��0���ؼ����1���պ���ָ��
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
	int _order;//B���Ľ״Σ�����Ϊ3
	BTNodePosi(T) _root;
	BTNodePosi(T) _hot;
	void solveOverflow(BTNodePosi(T));//�����������ķ��Ѵ���
	void solveUnderflow(BTNodePosi(T));//��ɾ���������ĺϲ�����
public:
	BTree(int order = 3) :_order(order), _size(0) {
		_root = new BTNode<T>();
	}

	BTree() {
		if (_root)
			release(_root);//�ͷ����нڵ�
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
		Rank r = v->key.search(e);//�ڵ�ǰ�ڵ�Ѱ�Ҳ�С��e�����ؼ���
		if (0 <= r && v->key[r] == e)
			return v;
		this->_hot = v;
		v = v->child[r + 1];//ת��������IO������
	}
	return nullptr;
}

template <typename T>
bool BTree<T>::insert(const T& e) {
	BTNodePosi(T) v = search(e);
	if (v)//�Ѵ���
		return false;
	Rank r = this->_hot->key.search(e);//���Һ��ʵĲ���λ��
	this->_hot->key.insert(r + 1, e);
	this->_hot->child.insert(r + 2, nullptr);//��Ϊ��ʱ��Ȼ�����ⲿ�ڵ㣨ȫ�ǿսڵ㣩����սڵ㣬��Ч�������ֱ�Ӳ���
	++this->_size;
	solveOverflow(this->_hot);
	return true;
}

template <typename T>
void BTree<T>::solveOverflow(BTNodePosi(T) v) {
	if (v->child.size() <= _order) //�ݹ��
		return;

	Rank s = _order / 2;//��ʱ��_order == key.size() == child.size()+1
	BTNodePosi(T) u = new BTNode<T>();
	for (Rank j = 0; j < _order - s - 1; ++j) {//��v�Ұ�ڵ�ת����u
		u->child.insert(j, v->child.remove(s + 1));
		u->key.insert(j, v->key.remove(s + 1));
	}
	u->child[_order - s - 1] = v->child.remove(s + 1);//���Ҳຢ�ӣ�ֻ�ƶ����ӣ����ƶ��ؼ��룩
	for (Rank j = 0; j < _order - s && u->child[j]; ++j)
		u->child[j]->parent = u;
	BTNodePosi(T) p = v->parent;
	if (!p) {//�������ڵ�
		_root = p = new BTNode<T>;
		p->child[0] = v;
		v->parent = p;
	}
	Rank r = 1 + p->key.search(v->key[0]);
	p->key.insert(r, v->key.remove(s));
	p->child.insert(r + 1, u);
	u->parent = p;

	solveOverflow(p);//����ݹ�logn�㣨���ߣ�
}

template <typename T>
bool BTree<T>::remove(const T& e) {
	BTNodePosi(T) v = search(e);
	if (!v)//�ڵ㲻����
		return false;

	Rank r = v->key.search(e);
	if (v->child[0]) {
		BTNodePosi(T) u = v->child[r + 1];
		while (u->child[0])//����������һֱ����
			u = u->child[0];//�ҵ�e�ĺ��
		v->key[r] = u->key[0];//����֮����λ��
		v = u; r = 0;
	}//���ˣ�v��Ȼλ����ײ㣬�����е�r���ؼ�����Ǵ�ɾ����
	v->key.remove(r);
	v->child.remove(r + 1);//ͬ�����к��Ӷ��ǿգ���˿��Ե�ЧΪֱ��ɾ�����һ��
	--this->_size;
	solveUnderflow(v);

	return true;
}

template <typename T>
void BTree<T>::solveUnderflow(BTNodePosi(T) v) {
	if ((this->_order + 1) / 2 <= v->child.size())//�ݹ��
		return;

	BTNodePosi(T) p = v->parent;
	if (!p) {//�ѵ����ڵ�
		if (0 == v->key.size() && v->child[0]) {//����v�Ѳ������ؼ��룬ȴ����Ψһ�ķǿպ���
			this->_root = v->child[0];
			this->_root->parent = nullptr;
			v->child[0] = nullptr;
			release(v);
		}
		return;
	}

	Rank r = 0;
	while (p->child[r] != v)
		++r;//ȷ��v��p�ĵ�r������

	//���1�������ֵܽ�ؼ���
	if (0 < r) {
		BTNodePosi(T) ls = p->child[r - 1];//���ֵ�
		if ((_order + 1) / 2 < ls->child.size()) {//���ֵ����㹻��ĺ��ӣ����Խ��1��
			v->key.insert(0, p->key[r - 1]);//p���һ���ؼ����v����Ϊ��С�ؼ��룩
			p->key[r - 1] = ls->key.remove(ls->key.size() - 1);//ls�����ؼ���ת��p
			v->child.insert(0, ls->child.remove(ls->child.size() - 1));//ls�����Ҳຢ�ӹ��̸�v

			if (v->child[0])
				v->child[0]->parent = v;
			return;
		}
	}
	//���2�������ֵܽ�ؼ���
	if (r < p->child.size() - 1) {
		BTNodePosi(T) rs = p->child[r + 1];//���ֵ�
		if ((_order + 1) / 2 < rs->child.size()) {//���ֵ����㹻��Ľڵ�
			v->key.insert(v->key.size(), p->key[r]);//p���һ���ؼ����v����Ϊ���ؼ��룩
			p->key[r] = rs->key.remove(0);//rs����С�ؼ���ת��p
			v->child.insert(v->child.size(), rs->child.remove(0));//��rs�������ӹ��̸�v

			if (v->child[v->child.size() - 1])
				v->child[v->child.size() - 1]->parent = v;
			return;
		}
	}
	//���3�������ֵ�ҪôΪ�գ�Ҫô��̫��
	if (0 < r) {//�����ֵܺϲ�
		BTNodePosi(T) ls = p->child[r - 1];
		//p�ĵ�r-1���ؼ���ת��ls��v������p�ĵ�r������
		ls->key.insert(ls->key.size(), p->key.remove(r - 1));
		p->child.remove(r);
		ls->child.insert(ls->child.size(), v->child.remove(0));
		//v������ຢ�ӹ��̸�ls�����Ҳຢ��
		if (ls->child[ls->child.size() - 1])
			ls->child[ls->child.size() - 1]->parent = ls;
		//vʣ��ؼ���ͺ���һ��ת��ls
		while (!v->key.empty()) {
			ls->key.insert(ls->key.size(), v->key.remove(0));
			ls->child.insert(ls->child.size(), v->child.remove(0));
			if (ls->child[ls->child.size() - 1])
				ls->child[ls->child.size() - 1]->parent = ls;
		}
		release(v);
	}
	else {//���ֵܺϲ�
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