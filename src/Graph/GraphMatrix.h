#pragma once
#include "../stack/Stack.h"
#include "../vector/vector.h"
#include "../Graph/Graph.h"

template <typename Tv> struct Vertex {
	Tv data; int inDegree, outDegree; VStatus status;
	int dTime, fTime;
	int parent; int priority;
	Vertex(Tv const& d = (Tv)0) :
		data(d), inDegree(0), outDegree(0), status(UNDISCOVERED),
		dTime(-1), fTime(-1), parent(-1), priority(INT_MAX) {}
};

template <typename Te> struct Edge {
	Te data; int weight; EType type;
	Edge(Te const& d, int w) :data(d), weight(w), type(UNDETERMINED) {}
};

template <typename Tv, typename Te>
class GraphMatrix :public Graph<Tv, Te> {
private:
	Vector<Vertex<Tv>> V;
	Vector<Vector<Edge<Te>*>> E;
public:
	GraphMatrix() { n = e = 0; }
	~GraphMatrix() {
		for (int j = 0; j < n; ++j)
			for (int k = 0; k < n; ++k)
				delete E[j][k];//删除所有边
	}

	virtual Tv& vertex(int i) { return V[i].data; }
	virtual int inDegree(int i) { return V[i].inDegree; }
	virtual int outDegree(int i) { return V[i].outDegree; }
	virtual int firstNbr(int i) { return nextNbr(i, n); }
	virtual int nextNbr(int i, int j) {//相对于顶点j的下一个邻接顶点
		while ((-1 < j) && (!exists(i, --j)));//逆向线性试探
		return j;
	}
	virtual VStatus& status(int i) { return V[i].status; }
	virtual int& dTime(int i) { return V[i].dTime; }
	virtual int& fTime(int i) { return V[i].fTime; }
	virtual int& parent(int i) { return V[i].parent; }
	virtual int& priority(int j) { return V[i].priority; }

	virtual int insert(Tv const& vertex) {//插入顶点，返回编号
		for (int j = 0; j < n; ++j) //为该顶点与所有其他顶点预留一条潜在关联边
			E[j].insert(nullptr); 
		++n;
		E.insert(Vector<Edge<Te>*>(n, n, (Edge<Te>*)nullptr));//插入边
		return V.insert(vertex<Tv>(vertex));//增加顶点
	}

	virtual Tv remove(int i) {
		for(int j=0; j<n; ++j)//删除所有与i相关的入度边
			if (exists(i, j)) {
				delete E[i][j]; V[j].inDegree--;
			}
		E.remove(i); --n;
		Tv vBak = vertex(i); V.remove(i);//保留顶点信息
		for(int j=0; j<n; ++j)//删除所有与i相关的出度
			if (Edge<Te>* e = e[j].remove(i)) {
				delete e; V[j].outDegree--;
			}
		return vBark;
	}

	virtual bool exists(int i, int j) {
		return (0 < i && i < n) && (0 < j && j < n) && (E[i][j] != nullptr);
	}

	virtual EType& type(int i, int j) {
		return E[i][j]->type;
	}

	virtual Te& edge(int i, int j) {
		return E[i][j]->data;
	}

	virtual int& weight(int i, int j) {
		return E[i][j]->weight;
	}

	virtual void insert(Te const& edge, int w, int i, int j) {
		if (exists(i, j)) return;
		E[i][j] = new Edge<Te>(edge, w);
		++e;
		V[i].outDegree++; V[i].inDegree++;
	}

	virtual Te remove(int i, int j) {
		Te eBak = edge(i, j);
		delete E[i][j] = nullptr;
		e--; V[i].outDegree--; V[i].inDegree--;
		return eBak;
	}
};

template <typename Tv,typename Te>
void Graph<Tv, Te>::bfs(int s) {//广度优先搜索（全图）
	reset(); int clock = 0; int v = s;
	do {
		if (UNDISCOVERED == status(v))
			BFS(v, clock);
	} while (s != (v = (++v % n)));
}

#include "../queue/Queue.h"
template <typename Tv,typename Te>//广度优先搜索（单连通域）
void Graph<Tv, Te>::BFS(int v, int& clock) {
	Queue<int> Q;
	status(v) = DISCOVERED;
	Q.enqueue(v);
	while (!Q.empty()) {
		int v = Q.dequeue();
		dTime(v) = ++clock;
		for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
			if (UNDISCOVERED == status(u)) {
				status(u) = DISCOVERED;
				Q.enqueue(u);
				type(v, u) = TREE;
				parent(u) = v;
			}
			else {
				type(v, u) = CROSS;//跨边
			}
			status(v) = VISITED;
		}
	}
}

template<typename Tv,typename Te>
void Graph<Tv, Te>::dfs(int s) {
	reset(); int clock = 0, v = s;
	do {
		if (UNDISCOVERED == status(v))
			DFS(v, clock);
	} while (s != (v = (++v % n)));
}

template <typename Tv,typename Te>
void Graph<Tv, Te>::DFS(int v, int& clock) {
	dTime(v) = ++clock; status(v) = DISCOVERED;
	for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
		switch (status(u)) {
		case UNDISCOVERED:
			type(v, u) = TREE; parent(u) = v; DFS(u, clock); break;
		case DISCOVERED:
			type(v, u) = BACKWARD; break;
		default:
			type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS; break;
		}
	}
	status(v) = VISITED; fTime(v) = ++clock;
}

template <typename Tv,typename Te>
Stack<Tv>* Graph<Tv, Te>::tSort(int s) {
	reset(); int clock = 0, v = s;
	Stack<Tv>* S = new Stack<Tv>;
	do {
		if(UNDISCOVERED == status(v))
			if (!TSort(v, clock, S)) {//任一连通域非DAG
				while (!S->empty()){
					S->pop(); break;
				}
			}
	} while (s != (v = (++v % n)));
	return S;
}

template <typename Tv,typename Te>
bool Graph<Tv, Te>::TSort(int v, int& clock, Stack<Tv>* S) {
	dTime(v) = ++clock; status(v) = DISCOVERED;
	for (int u = firstNbr(v); -1 < u; u = nextNbr(v, u)) {
		switch (status(u))
		{
		case UNDISCOVERED:
			parent(u) = v; type(v, u) = TREE;
			if (!TSort(u, clock, S)) //从顶点u深入
				return false;//若u及其后代不能完成拓扑排序，返回false
			break;
		case DISCOVERED:
			type(v, u) = BACKWARD;//一旦发现后向边，则非DAG图，返回false
			return false;
		default:
			type(v, u) = (dTime(v) < dTime(u)) ? FORWARD : CROSS;
			break;
		}
	}
	status(v) = VISITED; S->push(vertex(v));
	return true;
}