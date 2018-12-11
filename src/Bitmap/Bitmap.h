#pragma once
#include <string>
using namespace std;

class Bitmap
{
private:
	char* M;
	int N;//容量：N*sizeof(M)*8
protected:
	void init(int n) {
		M = new char[N = (n + 7) / 8];
		memset(M, 0, N);
	}
public:
	Bitmap(int n = 8) { init(n); }
	Bitmap(char* file, int n) {
		init(n);
		FILE* fp = fopen(file, "r");
		fread(M, sizeof(char), N, fp);
		fclose(fp);
	}
	~Bitmap() {
		delete[] M;
		M = nullptr;
	}

	void expand(int n) {
	}

	void set(int k) {
		expand(k);
		//k>>3：得到k在M的第几个元素
		M[k >> 3] |= (0x80 >> (k & 0x07));//得到k所表示的是当前字节的第几位
	}
	void clear(int k) {
		expand(k);
		M[k >> 3] &= ~(0x80 >> (k & 0x07));
	}

	bool test(int k) {
		expand(k);
		return M[k >> 3] & (0x80 >> (k & 0x07));
	}

	void dump(char* file) {
		FILE* fp = fopen(file, "w");
		fwrite(M, sizeof(char), N, fp);
		fclose(fp);
	}

	char* bits2string(int n) {
		expand(n - 1);
		char* s = new char[n + 1];
		s[n] = '\0';
		for (int i = 0; i < n; ++i)
			s[i] = test(i) ? '1' : '0';
		return s;
	}
};

