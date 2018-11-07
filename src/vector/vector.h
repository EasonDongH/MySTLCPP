#pragma once

typedef int Rank;                                          //��
#define DEFAULT_CAPACITY 3                                //Ĭ�ϳ�ʼ����

template <typename T> class Vector {
protected:
	Rank _size; int _capacity; T* _elem;                   //��ģ��������������
	void copyFrom(T const* A, Rank lo, Rank hi);           //������������A[lo,hi]
	void expand();                                         //�ռ䲻��ʱ����
	void shrik();                                          //������ӹ�Сʱ����
	Rank max(Rank lo, Rank hi);                            //ѡȡ����[lo,hi]���Ԫ��
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
	/************************���캯����ʼ****************************/
	Vector(int c=DEFAULT_CAPACITY,int s=0,T v=0) {       //����c����ģs����ʼֵv
		_elem = new T[c];
		for (_size = 0; _size < s; _size++)
			_elem[_size] = v;
	}
	Vector(T* A, Rank n) {                                //�������帴��
		copyFrom(A, 0, n);
	}
	Vector(T* A, Rank lo, Rank hi) {                      //����[lo,hi)���临��
		copyFrom(A, lo, hi);
	}
	Vector(Vector<T> const &V) {                          //�������帴��
		copyFrom(V._elem, 0, V._size);
	}
	Vector(Vector<T> const &V, Rank lo, Rank hi) {        //����[lo,hi)���临��
		copyFrom(V._elem, lo, hi);
	}
	/************************���캯������****************************/
	/************************����������ʼ****************************/
	~Vector() {
		delete[] _elem;
	}
	/************************������������****************************/
	/************************ֻ���ӿڿ�ʼ****************************/
	Rank size() const {                                    //���ص�ǰ��ģ
		return this->_size;                
	}
	bool empty() const {                                   //�п�
		return this->_size == 0;
	}
	bool disOrdered() const;                               //�ж������Ƿ�������            
	Rank find(T const & e) const {                         //���������������
		return find(e, 0, this->_size);
	}
	Rank find(T const & e, Rank lo, Rank hi) const;        //��������[lo,hi)�������
	Rank search(T const& e) const {                        //���������������
		return (0 >= _size) ? -1 : search(e, 0, _size);
	}
	Rank search(T const& e,Rank lo,Rank hi) const;         //��������[lo,hi)�������
	/************************ֻ���ӿڽ���****************************/
	/************************��д�ӿڽ���****************************/
	T& operator[](Rank r) const;                           //�����±������
	Vector<T> & operator =(Vector<T> const&);              //���ظ�ֵ���������������������¡
	T remove(Rank r);                                      //ɾ����Ϊr��Ԫ��
    int remove(Rank lo, Rank hi);                          //ɾ��[lo,hi)�����Ԫ��
	Rank insert(Rank r, T const& e);                       //��Ԫ�ز��뵽��Ϊr��λ��
	Rank insert(T const& e) {                              //��Ԫ�ز��뵽�������λ�ã�����push_back
		return insert(_size, e);
	}
	Rank push_back(T const& e) {                           //ģ��STL
		return insert(e);
	}
	void sort(Rank lo, Rank hi);                           //[lo,hi)��������
	void sort() {                                          //������������
		sort(0, _size);
	}
	void unsort(Rank lo, Rank hi);                         //[lo,hi)��������
	void unsort() {                                        //�����������
		unsort(0, _size);
	}
	int deduplicate();                                     //��������ȥ��
	int uniquify();                                        //��������ȥ��
	/************************��д�ӿڽ���****************************/
	/**************************������ʼ******************************/
	void traverse(void(*visit)(T&));                            //���ú���ָ�����������ÿ��Ԫ�أ����޸ģ�
	template <typename VST> void traverse(VST&);           //���ú����������������ÿ��Ԫ�أ����޸ģ�
	/**************************��������******************************/
};
