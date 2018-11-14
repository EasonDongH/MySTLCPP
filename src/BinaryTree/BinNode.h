#pragma once
#define BinNodePosi(T) BinNode<T>*
#define stature(p) ((p)?(p)->height : -1)  //结点高度：空树高度为1
typedef enum {RB_RED, RB_BLACK}RBColor;

template <typename T> struct BinNode {
	T data;
	BinNodePosi(T) parent; BinNodePosi(T) lc; BinNodePosi(T) rc;
	int height;
	int npl;                               //Null Path Length：左氏堆适用
	RBColor color;                         //红黑树

	BinNode():parent(nullptr),lc(nullptr),rc(nullptr),height(0),npl(1),color(RB_RED){}
	BinNode(T e, BinNodePosi(T) p=nullptr,BinNodePosi(T) lc=nullptr,BinNodePosi(T) rc=nullptr,int h=0, int npl=1, RBColor c=RB_RED)
		:data(e),parent(p),lc(lc),rc(rc),height(h),npl(npl),color(c){}

	int size();                                   //统计当前节点后代总数，即以该节点为根的子树
	BinNodePosi(T) insertAsLC(const T& e);
	BinNodePosi(T) insertAsRC(const T& e);
	BinNodePosi(T) succ();                       //取当前节点的直接后继
	template <typename VST> void travLevel(VST&);
	template <typename VST> void travPre(VST&);
	template <typename VST> void travIn(VST&);
	template <typename VST> void travPost(VST&);

	bool operator <(BinNode const& bn){return data < bn.data}
	bool operator ==(BinNode const& bn){return data == bn.data}
};

template <typename T>
int getNum(BinNodePosi(T) root) {
	if (root == nullptr) return 0;
	int left = getNum(root->lc);
	int right = getNum(root->rc);
	return left + right + 1;
}

template <typename T>
int BinNode<T>::size() {
	return getNum(this);
}

template <typename T>
BinNodePosi(T) BinNode<T>::insertAsLC(const T& e) {
	return this->lc = new BinNode(e, this);
}

template <typename T>
BinNodePosi(T) BinNode<T>::insertAsRC(const T& e) {
	return this->rc = new BinNode(e, this);
}