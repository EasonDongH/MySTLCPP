#pragma once
#include "Dictionary.h"

template <typename K, typename V> class Hashtable : public Dictionary<K, V> {
private:
	Entry<K, V>** ht;
	int M, N;//Ͱ��������������
	Bitmap* lazyRemoval;//����ɾ�����
#define lazilyRemoved(x) (lazyRemoval->test(x))
#define markAsRemoved(x) (lazyRemoval->set(x))

protected:
	int probe4Hit(const K& k);//�عؼ���k��Ӧ�Ĳ��������ҵ�����ƥ���Ͱ
	int probe4Free(const K& k);//�عؼ���k��Ӧ�Ĳ��������ҵ��׸����õĿ�Ͱ
	void rehash();//��ɢ��

public:
	Hashtable(int c = 5);//����һ��������С��c��������С��ɢ�б�
	~Hashtable();
	int size() const { return N; }
	bool put(K, V);
	V* get(K k);
	bool remove(K k);
};
