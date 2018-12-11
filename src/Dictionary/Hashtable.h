#pragma once
#include "Dictionary.h"

template <typename K, typename V> class Hashtable : public Dictionary<K, V> {
private:
	Entry<K, V>** ht;
	int M, N;//桶数量、词条数量
	Bitmap* lazyRemoval;//懒惰删除标记
#define lazilyRemoved(x) (lazyRemoval->test(x))
#define markAsRemoved(x) (lazyRemoval->set(x))

protected:
	int probe4Hit(const K& k);//沿关键码k对应的查找链，找到词条匹配的桶
	int probe4Free(const K& k);//沿关键码k对应的查找链，找到首个可用的空桶
	void rehash();//重散列

public:
	Hashtable(int c = 5);//创建一个容量不小于c的素数大小的散列表
	~Hashtable();
	int size() const { return N; }
	bool put(K, V);
	V* get(K k);
	bool remove(K k);
};
