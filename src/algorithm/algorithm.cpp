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
			N[j] = t; //�˴��ɸĽ�Ϊ��N[j] = (P[j] != P[t] ? t : N[t]);
		}
		else
			t = N[t];
	}
	return N;
}

int BM(char* P, char* T) {                //Boyer-Morre�㷨
	int* bc = buildBC(P);
	int* gs = buildGS(P);
	size_t i = 0;                                  //ģʽ��������ı�������ʼλ�ã���ʼʱ���ı�������룩
	while (i + strlen(P) <= strlen(T)) {
		int j = strlen(P) - 1;                   //��ģʽ����ĩβ�ַ���ʼ�Ա�
		while (P[j] == T[i + j])
			if (--j < 0)
				break;
		if (j < 0)                                       //�Ѿ���ȫƥ��
			break;
		else                                             //�����ʵ����ƶ�ģʽ��
			i += max(gs[j], j - bc[T[i + j]]);//λ��������BC���GS��ѡ�������
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
		bc[P[j]] = j;//�����㷨���ú���ֵ����ǰ��
	return bc;
}

int* buildGS(char* P) {//����ú�׺λ������O(m)
	int* ss = buildSS(P);
	size_t m = strlen(P);
	int* gs = new int[m];
	for (size_t j = 0; j < m; ++j)
		gs[j] = m;
	for (size_t i = 0, j = m - 1; j < UINT_MAX; --j) {//������һɨ����ַ�P[j]
		if (j + 1 == ss[j]) {//��P[0,j] = P[m-j-1,m)����
			while (i < m - j - 1)//����P[m-j-1]����ÿ���ַ�P[i]����
				gs[i++] = m - j - 1;//m-j-1����gs[i]��һ��ѡ��
		}
	}
	for (size_t j = 0; j < m - 1; ++j)
		gs[m - ss[j] - 1] = m - j - 1;
	delete[] ss;
	return gs;
}

int* buildSS(char* P) {//�������ƥ���׺���ȱ�O(m)
	int m = strlen(P);
	int* ss = new int[m];
	ss[m - 1] = m;//�����һ���ַ����ԣ���֮ƥ������׺��������P��
	//���£��ӵ����ڶ����ַ�����������ɨ��P�����μ����ss[]�������
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

