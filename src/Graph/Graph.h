#pragma once
#include "../stack/Stack.h"

typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus;//����״̬
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EType;//���ڱ�����������������

template <typename Tv, typename Te>
class Graph {
private:
	void reset() {
		for (int i = 0; i < n; ++i) {
			status(i) = UNDISCOVERED;//���ж���״̬��λ
			dTime(i) = fTime(i) = -1;
			parent(i) = -1; priority(i) = INT_MAX;
			for (int j = 0; j < n; ++j)//���б����͸�λ
				if (exists(i, j)) type(i, j) = UNDETERMINED;
		}
	}
	void BFS(int, int&);
	void DFS(int, int&);
	void BCC(int, int&, Stack<int>&);//����DFS��˫��ͨ�����ֽ��㷨
	bool TSort(int, int&, Stack<Tv>*);//����DFS�����������㷨
	template <typename PU> void PFS(int, PU);//���ȼ��������
public:
	Graph();
	~Graph();
	//����
	int n;
	virtual int insert(Tv const&) = 0;//���붥�㣬���ر��
	virtual Tv remove(int) = 0;//ɾ�����㼰������ߣ����ظö�����Ϣ
	virtual Tv& vertex(int) = 0;//����v�����ݣ�����v���ڣ�
	virtual int inDegree(int) = 0;//����v�����
	virtual int outDegree(int) = 0;//����v�ĳ���
	virtual int firstNbr(int) = 0;//����v���׸��ڽӶ���
	virtual int nextNbr(int, int) = 0;//����v�ģ�����ڶ���j������һ���ڽӶ���
	virtual VStatus& status(int) = 0;//����v��״̬
	virtual int& dTime(int) = 0;//ʱ���ǩ
	virtual int& fTime(int) = 0;//ʱ���ǩ
	virtual int& parent(int) = 0;//����v�ڱ������еĸ���
	virtual int& priority(int) = 0;//����v�ڱ������е����ȼ���
//�ߣ�Լ�����������ת��Ϊ�������һ������ߣ��Ӷ�������ͼ��������ͼ������
	int e;
	virtual bool exists(int, int) = 0;//��(v,u)�Ƿ����
	virtual void insert(Te const&, int, int, int) = 0;//�ڶ���v��u֮�����Ȩ��Ϊw�ı�e
	virtual Te remove(int, int) = 0;//ɾ������(v,u)֮��ıߣ����ظñ���Ϣ
	virtual EType& type(int, int) = 0;//��(v,u)����
	virtual Te& edge(int, int) = 0;//��(v,u)����
	virtual int& weight(int, int) = 0;//��(v,u)Ȩ��

	void bfs(int);
	void dfs(int);
	void bcc(int);//����DFS��˫��ͨ�����ֽ��㷨
	Stack<Tv>* tSort(int);//����DFS�����������㷨
	void prim(int);//��С֧����Prim�㷨
	void dijkstra(int);//���·��Dijkstra�㷨
	template <typename PU> void pfs(int, PU);//���ȼ��������
};

