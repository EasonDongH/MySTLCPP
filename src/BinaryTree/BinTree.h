#pragma once
#include "BinNode.h"

#define release(x) (delete (x))

template <typename T> class BinTree
{
private:
	int max(const int& n1, const int& n2) {
		if (n1 <= n2)
			return n2;
		else
			return n1;
	}
protected:
	int _size; 
	virtual int updateHeight(BinNodePosi(T) x);   //���½ڵ�x�ĸ߶�
	void updateHeightAbove(BinNodePosi(T) x);     //���½ڵ�x�������ȵĸ߶�
public:
	BinNodePosi(T) _root;
	BinTree() :_size(0), _root(nullptr) {}

	~BinTree() { if (0 < _size) remove(_root); }

	int size() const { return this->_size; }
	bool empty() const { return !this->_root; }
	BinNodePosi(T) root() const { return this->_root; }
	BinNodePosi(T) insertAsRoot(const T& e);
	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, const T& e);       //��e��x�����Ӳ��루ԭ�ޣ�
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, const T& e);
	BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>* &T);   //��T��Ϊx������������
	BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>* &T);
	int remove(BinNodePosi(T) x);                                  //ɾ����xΪ��������������x�����Ĺ�ģ
	BinTree<T>* secde(BinNodePosi(T) x);                           //������xժ������תΪһ�Ŷ���������
	
	bool operator <(const BinTree<T>& t) {
		return this->_root && t._root && lt(this->_root, t._root);
	}
	bool operator ==(const BinTree<T>& t) {
		return this->_root && t._root && (this->_root == t._root);
	}
};