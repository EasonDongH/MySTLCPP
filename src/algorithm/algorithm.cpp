#include "Algorithm.h"
#include <algorithm>
using namespace std;

template <typename T>
int Algorithm<T>::KMP(char* P, char* T) {
	int* next = buildNext(P);
	int n = (int)strlen(T), i = 0;
	int m = (int)strlen(P), j = 0;
	while (i < n && j < m) {
		if (j < 0 || T[i] == T[j]) {
			++i; ++j;
		}
		else
			j = next[j];
	}
	delete[] next;
	if (m <= (i - j))
		return i - j;
	else
		return -1;
}

int* buildNext(char* P) {
	size_t m = strlen(P), j = 0;
	int* N = new int[m];
	int t = N[0] = -1;
	while (j < m - 1) {
		if (t < 0 || P[j] == P[t]) {
			++j; ++t;
			N[j] = t; //此处可改进为：N[j] = (P[j] != P[t] ? t : N[t]);
		}
		else
			t = N[t];
	}
	return N;
}

int BM(char* P, char* T) {                //Boyer-Morre算法
	int* bc = buildBC(P);
	int* gs = buildGS(P);
	size_t i = 0;                                  //模式串相对于文本串的起始位置（初始时与文本串左对齐）
	while (i + strlen(P) <= strlen(T)) {
		int j = strlen(P) - 1;                   //从模式串的末尾字符开始对比
		while (P[j] == T[i + j])
			if (--j < 0)
				break;
		if (j < 0)                                       //已经完全匹配
			break;
		else                                             //否则，适当地移动模式串
			i += max(gs[j], j - bc[T[i + j]]);//位移量根据BC表和GS表选择最大者
	}
	delete[] gs;
	delete[] bc;
	if (i + strlen(P) <= strlen(T))
		return i;
	else
		return -1;
}

int* buildBC(char* P) {
	int* bc = new int[256];
	for (size_t j = 0; j < 256; ++j)
		bc[j] = -1;
	for (size_t m = strlen(P), j = 0; j < m; ++j)
		bc[P[j]] = j;//画家算法：用后来值覆盖前者
	return bc;
}

int* buildGS(char* P) {//构造好后缀位移量表：O(m)
	int* ss = buildSS(P);
	size_t m = strlen(P);
	int* gs = new int[m];
	for (size_t j = 0; j < m; ++j)
		gs[j] = m;
	for (size_t i = 0, j = m - 1; j < UINT_MAX; --j) {//逆向逐一扫描个字符P[j]
		if (j + 1 == ss[j]) {//若P[0,j] = P[m-j-1,m)，则
			while (i < m - j - 1)//对于P[m-j-1]左侧的每个字符P[i]而言
				gs[i++] = m - j - 1;//m-j-1都是gs[i]的一种选择
		}
	}
	for (size_t j = 0; j < m - 1; ++j)
		gs[m - ss[j] - 1] = m - j - 1;
	delete[] ss;
	return gs;
}

int* buildSS(char* P) {//构造最大匹配后缀长度表：O(m)
	int m = strlen(P);
	int* ss = new int[m];
	ss[m - 1] = m;//对最后一个字符而言，与之匹配的最长后缀就是整个P串
	//以下：从倒数第二个字符起自右向左扫描P，依次计算出ss[]其余各项
	for (int lo = m - 1, hi = m - 1, j = lo - 1; 0 <= j; --j) {
		if ((lo < j) && (ss[m - hi + j - 1] <= j - lo))
			ss[j] = ss[m - hi + j - 1];
		else {
			hi = j; lo = min(lo, hi);
			while ((0 <= lo) && (P[lo] == P[m - hi + lo - 1]))
				--lo;
			ss[j] = hi - lo;
		}
	}
	return ss;
}

