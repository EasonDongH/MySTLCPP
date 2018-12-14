#include "Algorithm.h"

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
	return i - j;
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