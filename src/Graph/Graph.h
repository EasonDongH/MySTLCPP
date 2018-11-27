#pragma once
#include "../stack/Stack.h"

typedef enum { UNDISCOVERED, DISCOVERED, VISITED } VStatus;//顶点状态
typedef enum { UNDETERMINED, TREE, CROSS, FORWARD, BACKWARD } EType;//边在遍历树中所属的类型

template <typename Tv, typename Te>
class Graph {
private:
	void reset() {
		for (int i = 0; i < n; ++i) {
			status(i) = UNDISCOVERED;//所有顶点状态复位
			dTime(i) = fTime(i) = -1;
			parent(i) = -1; priority(i) = INT_MAX;
			for (int j = 0; j < n; ++j)//所有边类型复位
				if (exists(i, j)) type(i, j) = UNDETERMINED;
		}
	}
	void BFS(int, int&);
	void DFS(int, int&);
	void BCC(int, int&, Stack<int>&);//基于DFS的双连通分量分解算法
	bool TSort(int, int&, Stack<Tv>*);//基于DFS的拓扑排序算法
	template <typename PU> void PFS(int, PU);//优先级搜索框架
public:
	Graph();
	~Graph();
	//顶点
	int n;
	virtual int insert(Tv const&) = 0;//插入顶点，返回编号
	virtual Tv remove(int) = 0;//删除顶点及其关联边，返回该顶点信息
	virtual Tv& vertex(int) = 0;//顶点v的数据（顶点v存在）
	virtual int inDegree(int) = 0;//顶点v的入度
	virtual int outDegree(int) = 0;//顶点v的出度
	virtual int firstNbr(int) = 0;//顶点v的首个邻接顶点
	virtual int nextNbr(int, int) = 0;//顶点v的（相对于顶点j）的下一个邻接顶点
	virtual VStatus& status(int) = 0;//顶点v的状态
	virtual int& dTime(int) = 0;//时间标签
	virtual int& fTime(int) = 0;//时间标签
	virtual int& parent(int) = 0;//顶点v在遍历树中的父亲
	virtual int& priority(int) = 0;//顶点v在遍历树中的优先级数
//边：约定：将无向边转换为方向互逆的一对有向边，从而将无向图视作有向图的特例
	int e;
	virtual bool exists(int, int) = 0;//边(v,u)是否存在
	virtual void insert(Te const&, int, int, int) = 0;//在顶点v和u之间插入权重为w的边e
	virtual Te remove(int, int) = 0;//删除顶点(v,u)之间的边，返回该边信息
	virtual EType& type(int, int) = 0;//边(v,u)类型
	virtual Te& edge(int, int) = 0;//边(v,u)数据
	virtual int& weight(int, int) = 0;//边(v,u)权重

	void bfs(int);
	void dfs(int);
	void bcc(int);//基于DFS的双连通分量分解算法
	Stack<Tv>* tSort(int);//基于DFS的拓扑排序算法
	void prim(int);//最小支撑树Prim算法
	void dijkstra(int);//最短路径Dijkstra算法
	template <typename PU> void pfs(int, PU);//优先级搜索框架
};

