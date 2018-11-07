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
	if (this->_size < this->_capacity) return;            //��������
	if (this->_capacity < DEFAULT_CAPACITY) {             //��СΪĬ������
		this->_capacity = DEFAULT_CAPACITY;
		return;
	}
	T *oldElem = this->_elem;                             //����һ����ʱ���鱣��Ԫ��
	this->_elem = new T[this->_capacity <<= 1];           //����������������
	for (int i = 0; i < this->_size; i++)                 //����Ԫ�ع�λ
		this->_elem[i] = oldElem[i];
	delete[] oldElem;                                     //��ʱ�ͷſռ�
}

template <typename T>
void Vector<T>::shrik() {                                 //��������趨������50%
	if (this->_capacity <= this->_size * 2) return;       //�������δ�ﵽ50%����������
	if (this->_capacity < DEFAULT_CAPACITY * 2) return;   //�������ܵ���Ĭ������

	T* oldElem = this->_elem;
	this->_elem = new T[this->_capacity >>= 1];           //��������
	for (int i = 0; i < this->_size; i++)
		this->_elem[i] = oldElem[i];

	delete[] oldElem;
}

template <typename T>
Rank Vector<T>::max(Rank lo, Rank hi) {
	T the_Max = hi;                                        //�ɺ���ǰɨ��
	while (lo < hi--) {                                    //��֤���ж�����ֵʱ�����ص���������
		if (this->_elem[the_Max] < this->_elem[hi])        //��Ч��֤�ڲ�������ʱ���ȶ���
			the_Max = hi;
	}

	return the_Max;
}

template <typename T>
Rank Vector<T>::bubble(Rank lo, Rank hi) {                   //ɨ�轻��һ��
	Rank last = lo;                                          //��¼���һ�ν�����λ��
	while (++lo < hi) {
		if (this->_elem[lo] < this->_elem[lo - 1]) {
			last = lo;                                       //�������һ�ν�����λ��
			swap(this->_elem[lo - 1], this->_elem[lo]);
		}
	}
	return last;                                             //������last==lo
}

template <typename T>
void Vector<T>::bubbleSort(Rank lo, Rank hi) {
	while (lo < hi)                                         //��Ԫ�ؽ���ɨ�轻��
		hi = bubble(lo, hi);                                //�������򣬽���ֱ���˳�
}

template <typename T>
bool Vector<T>::disOrdered() const {                       //����Ƿ�����Ĭ������
	for (int i = 1; i < this->_size; i++)
		if (this->_elem[i] < this->_elem[i - 1])
			return true;
	return false;
}

template <typename T>
Rank Vector<T>::find(T const& e, Rank lo, Rank hi) const { //�����������ң�������ͬԪ�أ�����������
	Rank i;
	for (i = hi- 1; lo <= i; i--)
		if (this->_elem[i] == e)
			break;
	return i;                                             //ʧ�ܷ���-1
}

template <typename T>
Rank Vector<T>::search(T const& e, Rank lo, Rank hi) const {//�������������ֲ���
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
	return -1;                                            //ʧ�ܷ���-1
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

	while (hi < this->_size)                              //ɾ�����䣺[lo,hi)
		this->_elem[lo++] = this->_elem[hi++];           //��ɾ����E[lo] == ԭE[hi]

	this->_size = lo;
	shrik();                                             //���б�Ҫ������
	return hi - lo;                                      //����ɾ������
}

template <typename T>
Rank Vector<T>::insert(Rank r, T const& e) {
	expand();                                            //����Ҫ������
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
void Vector<T>::unsort(Rank lo, Rank hi) {              //������ȸ��ʴ���
	T* V = this->_elem + lo;
	for (int i = hi - lo; 0 < i; i--)
		swap(V[i - 1], V[rand() % i]);
}

template <typename T>
int Vector<T>::deduplicate() {                         //��������ȥ��
	int i=1, oldSize = this->_size;
	while (i < this->_size) {                         //����ͬԪ�أ�������С��
		find(this->_elem[i], 0, i) < 0 ? i++ : remove(i);
	}

	return oldSize - this->_size;                            //����ɾ����Ԫ�ظ���
}

//template <typename T>
//int Vector<T>::uniquify() {                            //��������ȥ��
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
int Vector<T>::uniquify() {                            //��������ȥ��
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