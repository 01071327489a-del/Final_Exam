/*
파일명 : GraphAlgorithms.cpp
작성자 : 조성윤 (22312083)
작성일 : 2026-06-09

주요 기능 설명 :
- 그래프 탐색(DFS, BFS), 최단 경로(Dijkstra, Floyd-Warshall), 최소 신장 트리(Prim, Kruskal) 알고리즘을 구현합니다.
- dijkstraPath 함수는 출발지와 목적지 정점을 표에서 제외하여 출력하며,
*/

#include "GraphAlgorithms.h"
#include <algorithm>  
#include <iostream>
#include <iomanip>

// 경로를 역추적하여 시작점부터 도착점까지의 정점 ID 배열을 생성하는 헬퍼 함수
std::vector<int> GraphAlgorithms::buildPath(int start, int target, const std::vector<int>& previous) {
	std::vector<int> path;
	if (target < 0 || target >= static_cast<int>(previous.size())) return path;
	for (int cur = target; cur != -1; cur = previous[cur]) {
		path.push_back(cur);
		if (cur == start) break;
	}
	if (path.back() != start) return {};
	std::reverse(path.begin(), path.end());
	return path;
}

// 깊이 우선 탐색(DFS)
PathResult GraphAlgorithms::dfsPath(const Graph& graph, const std::string& startName, const std::string& targetName) {
	int start = graph.idOf(startName);
	int target = graph.idOf(targetName);
	int n = graph.vertexCount();
	std::vector<bool> visited(n, false);
	std::vector<int> previous(n, -1);
	std::function<bool(int)> dfs = [&](int u) {
		visited[u] = true;
		if (u == target) return true;
		for (const Edge& e : graph.adjacentEdges(u)) {
			int v = e.to;
			if (!visited[v]) {
				previous[v] = u;
				if (dfs(v)) return true;
			}
		}
		return false;
		};
	PathResult result;
	result.found = dfs(start);
	if (result.found) {
		result.path = buildPath(start, target, previous);
		result.distance = static_cast<double>(result.path.size() - 1);
	}
	return result;
}

// 너비 우선 탐색(BFS)
PathResult GraphAlgorithms::bfsPath(const Graph& graph, const std::string& startName, const std::string& targetName) {
	int start = graph.idOf(startName);
	int target = graph.idOf(targetName);
	int n = graph.vertexCount();
	std::queue<int> q;
	std::vector<bool> visited(n, false);
	std::vector<int> previous(n, -1);
	std::vector<int> level(n, -1);
	visited[start] = true;
	level[start] = 0;
	q.push(start);
	while (!q.empty()) {
		int u = q.front();
		q.pop();
		if (u == target) break;
		for (const Edge& e : graph.adjacentEdges(u)) {
			int v = e.to;
			if (!visited[v]) {
				visited[v] = true;
				previous[v] = u;
				level[v] = level[u] + 1;
				q.push(v);
			}
		}
	}
	PathResult result;
	result.found = visited[target];
	if (result.found) {
		result.path = buildPath(start, target, previous);
		result.distance = level[target];
	}
	return result;
}

// 다익스트라(Dijkstra) 알고리즘: 출발/도착지 제외 출력, 라운드별 정렬, 10라운드 공백 처리
PathResult GraphAlgorithms::dijkstraPath(const Graph& graph, const std::string& startName, const std::string& targetName) {
	int start = graph.idOf(startName);
	int target = graph.idOf(targetName);
	int n = graph.vertexCount();
	std::priority_queue<std::pair<double, int>, std::vector<std::pair<double, int>>, std::greater<std::pair<double, int>>> pq;
	std::vector<double> dist(n, INF);
	std::vector<int> previous(n, -1);
	dist[start] = 0.0;
	pq.push({ 0.0, start });
	int round = 0;

	std::cout << "Dijkstra Shortest Path Finding from " << startName << " to " << targetName << " ....\n";
	std::cout << "Dijkstra::Least Cost from Vertex (" << startName << ") at each round :\n";

	// 헤더 출력: 출발지와 목적지를 제외한 정점들만 출력
	std::cout << "       |";
	for (const Vertex& v : graph.vertices()) {
		if (v.name != startName && v.name != targetName) std::cout << std::setw(5) << v.name;
	}
	std::cout << "\n-------+" << std::string((n - 2) * 5, '-') << '\n';

	while (!pq.empty()) {
		auto [curDist, u] = pq.top();
		pq.pop();
		if (curDist > dist[u]) continue;

		if (u == target) {
			break;
		}

		for (const Edge& e : graph.adjacentEdges(u)) {
			if (dist[u] + e.weight < dist[e.to]) {
				dist[e.to] = dist[u] + e.weight;
				previous[e.to] = u;
				pq.push({ dist[e.to], e.to });
			}
		}

		
		std::cout << "round [" << std::setw(2) << round++ << "] |";
		for (int i = 0; i < n; ++i) {
			if (graph.vertex(i).name == startName || graph.vertex(i).name == targetName) continue;
			if (dist[i] == INF) std::cout << std::setw(5) << "+oo";
			else std::cout << std::setw(5) << (int)dist[i];
		}
		std::cout << " ==> selected vertex : " << graph.vertex(u).name << std::endl;
		

	}
	PathResult result;
	result.found = dist[target] != INF;
	result.distance = dist[target];
	if (result.found) result.path = buildPath(start, target, previous);
	return result;
}

// 플로이드-워셜 알고리즘
std::vector<std::vector<double>> GraphAlgorithms::floydWarshall(const Graph& graph) {
	auto dist = graph.adjacencyMatrix();
	int n = graph.vertexCount();
	for (int k = 0; k < n; ++k) {
		for (int i = 0; i < n; ++i) {
			if (dist[i][k] == INF) continue;
			for (int j = 0; j < n; ++j) {
				if (dist[k][j] == INF) continue;
				dist[i][j] = std::min(dist[i][j], dist[i][k] + dist[k][j]);
			}
		}
	}
	return dist;
}

// 프림 알고리즘: MST 생성 및 최종 가중치 합 출력
MSTResult GraphAlgorithms::primMST(const Graph& graph, const std::string& startName) {
	int start = graph.idOf(startName);
	int n = graph.vertexCount();
	std::vector<bool> selected(n, false);
	struct Candidate { double weight; Edge edge; bool operator>(const Candidate& other) const { return weight > other.weight; } };
	std::priority_queue<Candidate, std::vector<Candidate>, std::greater<Candidate>> pq;
	std::vector<double> dist(n, INF);
	dist[start] = 0;

	auto pushEdges = [&](int u) {
		selected[u] = true;
		for (const Edge& e : graph.adjacentEdges(u)) {
			if (!selected[e.to] && e.weight < dist[e.to]) {
				dist[e.to] = e.weight;
				pq.push(Candidate{ e.weight, e });
			}
		}
		};

	MSTResult result;
	pushEdges(start);
	while (!pq.empty() && result.edges.size() < (size_t)n - 1) {
		Candidate cur = pq.top();
		pq.pop();
		if (selected[cur.edge.to]) continue;

		result.edges.push_back(cur.edge);
		result.totalWeight += cur.edge.weight;
		pushEdges(cur.edge.to);
	}

	std::cout << "End of finding Minimum Spanning Tree by Prim-Jarnik's Algorithm: selectedEdgeLst_size = " << result.edges.size() << std::endl;
	std::cout << "Total weight = " << result.totalWeight << std::endl;

	result.connected = result.edges.size() == (size_t)n - 1;
	return result;
}

// 크루스칼 알고리즘에 필요한 Union-Find
class DisjointSet {
private:
	std::vector<int> p;
public:
	explicit DisjointSet(int n) : p(n) { std::iota(p.begin(), p.end(), 0); }
	int find(int x) { return p[x] == x ? x : (p[x] = find(p[x])); }
	bool unite(int a, int b) {
		int ra = find(a), rb = find(b);
		if (ra == rb) return false;
		p[ra] = rb;
		return true;
	}
};

// 크루스칼 알고리즘
MSTResult GraphAlgorithms::kruskalMST(const Graph& graph) {
	std::vector<Edge> edges = graph.edges(true);
	std::sort(edges.begin(), edges.end(), [](const Edge& a, const Edge& b) { return a.weight < b.weight; });
	DisjointSet ds(graph.vertexCount());
	MSTResult result;
	for (const Edge& e : edges) {
		if (ds.unite(e.from, e.to)) {
			result.edges.push_back(e);
			result.totalWeight += e.weight;
		}
	}
	result.connected = result.edges.size() == (size_t)graph.vertexCount() - 1;
	return result;
}