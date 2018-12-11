#include "Hashtable.h"
#include "../Bitmap/Bitmap.h"
#include <string>
using namespace std;

template <typename K, typename V> Hashtable<K, V>::Hashtable(int c) {
	M = primeNLT(c, 1048576, "../../_input/prime-1048576-bitmap.txt");//得到一个不小于c的素数
	N = 0;
	ht = new Entry<K, V>*[M];
	memset(ht, 0, sizeof(Entry < K<V>*)*M);
	lazyRemoval = new Bitmap(M);
}

int primeNLT(int c, int n, char* file) {
	Bitmap B(file, n);//记录了小于n的所有素数
	while (c < n) {
		if (B.test(c))
			++c;
		else
			return c;
	}
	return c;
}

template <typename K, typename V> Hashtable<K, V>::~Hashtable() {
	for (int i = 0; i < M; ++i)
		if (ht[i])
			release(ht[i]);
	release(ht);
	release(lazyRemoval);
}

template <typename K, typename V> V* Hashtable<K, V>::get(K k) {
	int r = probe4Hit(k);
	return ht[r] ? &(ht[r]->value) : nullptr;
}

template <typename K, typename V> int Hashtable<K, V>::probe4Hit(const K& k) {
	int r = hashCode(k) % M;
	while ((ht[r] && (k != ht[r]->key)) || (!ht[r] && lazilyRemoved(r)))
		r = (r + 1) % M;//线性探测
	return r;
}

template <typename K, typename V> bool Hashtable<K, V>::remove(K k) {
	int r = probe4Hit(k);
	if (!ht[r]) return false;//词条不存在
	release(ht[r]);
	ht[r] = nullptr;
	markAsRemoved(r);
	--N;
	return true;
}

template <typename K, typename V> bool Hashtable<K, V>::put(K k, V v) {
	if (ht[probe4Hit(k)])
		return false;//词条重复
	int r = probe4Free(k);//为新词条找个空桶
	ht[r] = new Entry<K, V>;
	++N;
	if (N * 2 > M) rehash();//填装因子大于50%，重散列
	return true;
}

template<typename K, typename V> int Hashtable<K, V>::probe4Free(const K& k) {
	int r = hashCode(k) % M;
	while (ht[r])
		r = (r + 1) % M;
	return r;
}

template<typename K, typename V> void Hashtable<K, V>::rehash() {
	int old_capacity = M;
	Entry<K, V>** old_ht = ht;
	M = primeNLT(2 * M, 1048576, "../../_input/prime-1048576-bitmap.txt");
	N = 0；
	ht = new Entry<K, V>*[M];
	memset(ht, 0, sizeof(Entry < K<V>*)*M);
	release(lazyRemoval);
	lazyRemoval = new Bitmap(M);
	for (int i = 0; i < old_capacity; ++i)//将原桶数据逐一插入至新桶
		if (old_ht[i])
			put(old_ht[i]->key, old_ht[i]->value);
	release(old_ht);
}