#include "List.h"
#include "List.cpp"
#include <random>
#include <iostream>
using namespace std;

template <typename T>
void visit(T& e) {
	cout << e << " ";
}

int main() {
	List<int> L;
	int i, size = 1337;
	//≤‚ ‘≈≈–Ú
	/*for (int i = 0; i < 10; i++)
		L.insertAsLast(rand() % 100);
	L.traverse(visit);
	cout << endl;
	L.sort();
	L.traverse(visit);
	cout << endl;
	cout << L.disOrdered();
	cout << endl;*/

	//≤‚ ‘∑≠◊™
	for ( i = 1; i <= size; i++)
		L.insertAsLast(i);
	L.traverse(visit);
	cout << endl;
	L.reverse();
	L.traverse(visit);
	cout << endl;
	
	return 0;
}