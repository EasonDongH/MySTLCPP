#include "Stack.h"
#include <iostream>
using namespace std;

int main() {
	int i, size = 10;
	Stack<int> S;
	for (i = 1; i <= size; ++i)
		S.push(i);
	while (!S.empty()) {
		int t = S.top();
		cout << t << " ";
		if (t != --i)
			break;
		S.pop();
	}

	return 0;
}