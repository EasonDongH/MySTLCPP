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
	virtual int updateHeight(BinNodePosi(T) x);   //更新节点x的高度
	void updateHeightAbove(BinNodePosi(T) x);     //更新节点x及其祖先的高度
public:
	BinNodePosi(T) _root;
	BinTree() :_size(0), _root(nullptr) {}

	~BinTree() { if (0 < _size) remove(_root); }

	int size() const { return this->_size; }
	bool empty() const { return !this->_root; }
	BinNodePosi(T) root() const { return this->_root; }
	BinNodePosi(T) insertAsRoot(const T& e);
	BinNodePosi(T) insertAsLC(BinNodePosi(T) x, const T& e);       //将e做x的左孩子插入（原无）
	BinNodePosi(T) insertAsRC(BinNodePosi(T) x, const T& e);
	BinNodePosi(T) attachAsLC(BinNodePosi(T) x, BinTree<T>* &T);   //将T作为x的左子树插入
	BinNodePosi(T) attachAsRC(BinNodePosi(T) x, BinTree<T>* &T);
	int remove(BinNodePosi(T) x);                                  //删除以x为根的子树，返回x子树的规模
	BinTree<T>* secde(BinNodePosi(T) x);                           //将子树x摘除，并转为一颗独立的子树
	
	bool operator <(const BinTree<T>& t) {
		return this->_root && t._root && lt(this->_root, t._root);
	}
	bool operator ==(const BinTree<T>& t) {
		return this->_root && t._root && (this->_root == t._root);
	}
};