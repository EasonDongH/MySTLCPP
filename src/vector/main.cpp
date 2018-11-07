#include <iostream>
#include "vector.h"
#include "vector.cpp"
using namespace std;

template <typename T>
void visit(T& e) {
	std::cout << e << " ";
}

int main() {
	//实例化
	Vector<int> V;
	V.push_back(int(1));
	V.traverse(visit); std::cout << std::endl;
	V.push_back(3); V.push_back(3); V.push_back(4); V.push_back(4);
	V.traverse(visit); std::cout << std::endl;
	//添加随机数
	for (int i = 0; i < 10; i++)
		V.push_back(std::rand() % 100);
	V.traverse(visit); std::cout << std::endl;
	//排序
	cout << "排序：" << endl;
	V.sort();
	V.traverse(visit); std::cout << std::endl;
	//检测是否有序
	if (!V.disOrdered())
		cout << "排序完成！" << endl;
	//打乱
	cout << "置乱：" << endl;
	V.unsort();
	V.traverse(visit); std::cout << std::endl;
	//检测是否有序
	if (V.disOrdered())
		cout << "排序未完成！" << endl;
	//无序去重
	cout << "无序去重：" << endl;
	cout << "原始序列：" << endl;
	V.traverse(visit); std::cout << std::endl;
	V.deduplicate();
	V.traverse(visit); std::cout << std::endl;
	//有序去重
	cout << "有序去重：" << endl;
	V.push_back(23); V.push_back(23);
	V.sort();
	cout << "原始序列：" << endl;
	V.traverse(visit); std::cout << std::endl;
	V.uniquify();
	V.traverse(visit); std::cout << std::endl;

	return 0;
}