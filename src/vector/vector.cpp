#include "vector.h"

template <typename T>
void Vector<T>::copyFrom(T const* A, Rank lo, Rank hi) {
	_elem = new T[_capacity = 2 * (hi - lo)];
	for (_size = 0; lo < hi; lo++, _size++) {
		_elem[_size] = A[lo];
	}
}

template <typename T>
void Vector<T>::expand() {
	if (this->_size < this->_capacity) return;            //无需扩容
	if (this->_capacity < DEFAULT_CAPACITY) {             //最小为默认容量
		this->_capacity = DEFAULT_CAPACITY;
		return;
	}
	T *oldElem = this->_elem;                             //借助一个临时数组保存元素
	this->_elem = new T[this->_capacity <<= 1];           //数据容量扩容两倍
	for (int i = 0; i < this->_size; i++)                 //数组元素归位
		this->_elem[i] = oldElem[i];
	delete[] oldElem;                                     //及时释放空间
}

template <typename T>
void Vector<T>::shrik() {                                 //填充因子设定不低于50%
	if (this->_capacity <= this->_size * 2) return;       //填充因子未达到50%，无需缩容
	if (this->_capacity < DEFAULT_CAPACITY * 2) return;   //容量不能低于默认容量

	T* oldElem = this->_elem;
	this->_elem = new T[this->_capacity >>= 1];           //容量减半
	for (int i = 0; i < this->_size; i++)
		this->_elem[i] = oldElem[i];

	delete[] oldElem;
}

template <typename T>
Rank Vector<T>::max(Rank lo, Rank hi) {
	T the_Max = hi;                                        //由后向前扫描
	while (lo < hi--) {                                    //保证当有多个最大值时，返回的是最大的秩
		if (this->_elem[the_Max] < this->_elem[hi])        //有效保证在插入排序时的稳定性
			the_Max = hi;
	}

	return the_Max;
}

template <typename T>
Rank Vector<T>::bubble(Rank lo, Rank hi) {                   //扫描交换一次
	Rank last = lo;                                          //记录最后一次交换的位置
	while (++lo < hi) {
		if (this->_elem[lo] < this->_elem[lo - 1]) {
			last = lo;                                       //更新最后一次交换的位置
			swap(this->_elem[lo - 1], this->_elem[lo]);
		}
	}
	return last;                                             //若有序，last==lo
}

template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi) {
	while (lo < hi)                                         //逐元素进行扫描交换
		hi = bubble(lo, hi);                                //若已有序，将会直接退出
}

template <typename T>
bool Vector<T>::disOrdered() const {                       //检查是否有序（默认升序）
	for (int i = 1; i < this->_size; i++)
		if (this->_elem[i] < this->_elem[i - 1])
			return true;
	return false;
}

template <typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const { //无序向量查找：若有相同元素，返回最大的秩
	Rank i;
	for (i = hi- 1; lo <= i; i--)
		if (this->_elem[i] == e)
			break;
	return i;                                             //失败返回-1
}

template <typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const {//有序向量：二分查找
	Rank mi;
	while (lo <= hi) {
		mi = (lo + hi) / 2;
		if (this->_elem[mi] < e)
			lo = mi + 1;
		else if (e < this->_elem[mi])
			hi = mi - 1;
		else
			return mi;
	}
	return -1;                                            //失败返回-1
}

template <typename T>
T& Vector<T>::operator[](Rank r) const {
	return this->_elem[r];
}

template <typename T>
Vector<T>& Vector<T>::operator=(Vector<T> const& V) {
	if (this->_elem) delete[] this->_elem;
	copyFrom(V, 0, V.size);
	return *this;
}

template <typename T>
T Vector<T>::remove(Rank r) {
	T t = this->_elem[r];
	remove(r, r + 1);
	this->_size--;
	return t;
}

template <typename T>
int Vector<T>::remove(Rank lo, Rank hi) {
	if (lo == hi) return 0;

	while (hi < this->_size)                              //删除区间：[lo,hi)
		this->_elem[lo++] = this->_elem[hi++];           //即删除后E[lo] == 原E[hi]

	this->_size = lo;
	shrik();                                             //若有必要则缩容
	return hi - lo;                                      //返回删除总量
}

template <typename T>
Rank Vector<T>::insert(Rank r, T const& e) {
	expand();                                            //若需要则扩容
	for (int i = this->_size; r <= i; i--)
		this->_elem[i] = this->_elem[i - 1];
	this->_elem[r] = e;
	this->_size++;
	return r;
}

template <typename T>
void Vector<T>::sort(Rank lo, Rank hi) {
	bubbleSort(lo, hi);
}

#include <random>
template <typename T>
void Vector<T>::unsort(Rank lo, Rank hi) {              //将数组等概率打乱
	T* V = this->_elem + lo;
	for (int i = hi - lo; 0 < i; i--)
		swap(V[i - 1], V[rand() % i]);
}

template <typename T>
int Vector<T>::deduplicate() {                         //无序向量去重
	int i=1, oldSize = this->_size;
	while (i < this->_size) {                         //有相同元素，保留最小秩
		find(this->_elem[i], 0, i) < 0 ? i++ : remove(i);
	}

	return oldSize - this->_size;                            //返回删除的元素个数
}

//template <typename T>
//int Vector<T>::uniquify() {                            //有序向量去重
//	int i, j, size = 0;
//	for (i = 0; i < this->_size; i=j) {
//		for (j = i + 1; j < this->size&&this->_elem[i] == this->_elem[j]; j++);
//		this->_elem[size++] = this->_elem[i];
//	}
//	this->_size = size;
//	shrik();
//	return i - size;
//}

template <typename T>
int Vector<T>::uniquify() {                            //有序向量去重
	int i = 0, j = 0;
	while (++j < this->_size) {
		if (this->_elem[i] != this->_elem[j])
			this->_elem[++i] = this->_elem[j];
	}
	this->_size = ++i;
	shrik();
	return j - i;
}

template <typename T>
void Vector<T>::traverse(void(*visit)(T&)) {
	for (int i = 0; i < this->_size; i++)
		visit(this->_elem[i]);
}

template <typename T> template <typename VST>
void Vector<T>::traverse(VST& visit) {
	for (int i = 0; i < this->_size; i++)
		visit(this->_elem[i]);
}