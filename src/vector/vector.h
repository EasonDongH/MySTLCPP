#pragma once

typedef int Rank;                                          //秩
#define DEFAULT_CAPACITY 3                                //默认初始容量

template <typename T> class Vector {
protected:
	Rank _size; int _capacity; T* _elem;                   //规模、容量、数据区
	void copyFrom(T const* A, Rank lo, Rank hi);           //复制数据区间A[lo,hi]
	void expand();                                         //空间不足时扩容
	void shrik();                                          //填充因子过小时缩容
	Rank max(Rank lo, Rank hi);                            //选取区间[lo,hi]最大元素
	void swap(T& t1, T& t2) {
		T tmp = t1; t1 = t2; t2 = tmp;
	}

	//bool bubble(Rank lo, Rank hi);
	Rank bubble(Rank lo, Rank hi);
	void bubbleSort(Rank lo, Rank hi);
	void selectionSort(Rank lo, Rank hi);
	void merge(Rank lo, Rank hi);
	void mergeSort(Rank lo, Rank hi);
	Rank partition(Rank lo, Rank hi);
	void quickSort(Rank lo, Rank hi);
	void heapSort(Rank lo, Rank hi);
public:
	/************************构造函数开始****************************/
	Vector(int c=DEFAULT_CAPACITY,int s=0,T v=0) {       //容量c、规模s、初始值v
		_elem = new T[c];
		for (_size = 0; _size < s; _size++)
			_elem[_size] = v;
	}
	Vector(T* A, Rank n) {                                //数组整体复制
		copyFrom(A, 0, n);
	}
	Vector(T* A, Rank lo, Rank hi) {                      //数据[lo,hi)区间复制
		copyFrom(A, lo, hi);
	}
	Vector(Vector<T> const &V) {                          //向量整体复制
		copyFrom(V._elem, 0, V._size);
	}
	Vector(Vector<T> const &V, Rank lo, Rank hi) {        //向量[lo,hi)区间复制
		copyFrom(V._elem, lo, hi);
	}
	/************************构造函数结束****************************/
	/************************析构函数开始****************************/
	~Vector() {
		delete[] _elem;
	}
	/************************析构函数结束****************************/
	/************************只读接口开始****************************/
	Rank size() const {                                    //返回当前规模
		return this->_size;                
	}
	bool empty() const {                                   //判空
		return this->_size == 0;
	}
	bool disOrdered() const;                               //判断向量是否已排序            
	Rank find(T const & e) const {                         //无序向量整体查找
		return find(e, 0, this->_size);
	}
	Rank find(T const & e, Rank lo, Rank hi) const;        //无序向量[lo,hi)区间查找
	Rank search(T const& e) const {                        //有序向量整体查找
		return (0 >= _size) ? -1 : search(e, 0, _size);
	}
	Rank search(T const& e,Rank lo,Rank hi) const;         //有序向量[lo,hi)区间查找
	/************************只读接口结束****************************/
	/************************可写接口结束****************************/
	T& operator[](Rank r) const;                           //重载下标操作符
	Vector<T> & operator =(Vector<T> const&);              //重载赋值操作符，便于向量整体克隆
	T remove(Rank r);                                      //删除秩为r的元素
    int remove(Rank lo, Rank hi);                          //删除[lo,hi)区间的元素
	Rank insert(Rank r, T const& e);                       //将元素插入到秩为r的位置
	Rank insert(T const& e) {                              //将元素插入到向量最后位置，类似push_back
		return insert(_size, e);
	}
	Rank push_back(T const& e) {                           //模仿STL
		return insert(e);
	}
	void sort(Rank lo, Rank hi);                           //[lo,hi)区间排序
	void sort() {                                          //向量整体排序
		sort(0, _size);
	}
	void unsort(Rank lo, Rank hi);                         //[lo,hi)区间置乱
	void unsort() {                                        //向量整体打乱
		unsort(0, _size);
	}
	int deduplicate();                                     //无序向量去重
	int uniquify();                                        //有序向量去重
	/************************可写接口结束****************************/
	/**************************遍历开始******************************/
	void traverse(void(*visit)(T&));                            //利用函数指针访问向量中每个元素（可修改）
	template <typename VST> void traverse(VST&);           //利用函数对象访问向量中每个元素（可修改）
	/**************************遍历结束******************************/
};
