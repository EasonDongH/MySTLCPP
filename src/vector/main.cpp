#include <iostream>
#include "vector.h"
#include "vector.cpp"
using namespace std;

template <typename T>
void visit(T& e) {
	std::cout << e << " ";
}

int main() {
	//ʵ����
	Vector<int> V;
	V.push_back(int(1));
	V.traverse(visit); std::cout << std::endl;
	V.push_back(3); V.push_back(3); V.push_back(4); V.push_back(4);
	V.traverse(visit); std::cout << std::endl;
	//��������
	for (int i = 0; i < 10; i++)
		V.push_back(std::rand() % 100);
	V.traverse(visit); std::cout << std::endl;
	//����
	cout << "����" << endl;
	V.sort();
	V.traverse(visit); std::cout << std::endl;
	//����Ƿ�����
	if (!V.disOrdered())
		cout << "������ɣ�" << endl;
	//����
	cout << "���ң�" << endl;
	V.unsort();
	V.traverse(visit); std::cout << std::endl;
	//����Ƿ�����
	if (V.disOrdered())
		cout << "����δ��ɣ�" << endl;
	//����ȥ��
	cout << "����ȥ�أ�" << endl;
	cout << "ԭʼ���У�" << endl;
	V.traverse(visit); std::cout << std::endl;
	V.deduplicate();
	V.traverse(visit); std::cout << std::endl;
	//����ȥ��
	cout << "����ȥ�أ�" << endl;
	V.push_back(23); V.push_back(23);
	V.sort();
	cout << "ԭʼ���У�" << endl;
	V.traverse(visit); std::cout << std::endl;
	V.uniquify();
	V.traverse(visit); std::cout << std::endl;

	return 0;
}