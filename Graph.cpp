/*
파일명 : Graph.cpp
작성자 : 조성윤 (22312083)
작성일 : 2026-06-09

주요 기능 설명 :
- Graph.h에서 정의된 Graph 클래스의 각 멤버 함수들을 구현한 소스 파일입니다.
- 정점 추가, 간선 연결, 파일 데이터 로드 및 그래프의 연결 상태를 행렬(Matrix)과 리스트(List)로 출력하는 기능을 담당합니다.
*/

#include "Graph.h"
#include <utility>

// 생성자: 그래프 이름과 방향성 여부를 초기화합니다.
Graph::Graph(std::string name, bool directed) : _graphName(std::move(name)), _directed(directed) {}

// 그래프의 이름과 방향성 여부를 반환합니다.
const std::string& Graph::name() const noexcept { return _graphName; }
bool Graph::isDirected() const noexcept { return _directed; }

// 그래프에 등록된 정점의 개수를 반환합니다.
int Graph::vertexCount() const noexcept { return static_cast<int>(_vertices.size()); }

// 그래프의 총 간선 개수를 계산합니다.
int Graph::edgeCount() const noexcept {
	int count = 0;
	// 모든 정점의 인접 리스트에 저장된 간선 개수를 합산합니다.
	for (const auto& edges : _adj) count += static_cast<int>(edges.size());
	// 무방향 그래프는 간선이 양방향으로 중복 저장되므로 개수를 2로 나눕니다.
	return _directed ? count : count / 2;
}

// 새로운 정점을 추가합니다. 이미 존재하면 기존 ID를 반환하고, 처음이면 새 ID를 부여합니다.
int Graph::addVertex(const std::string& vertexName) {
	auto found = _idByName.find(vertexName);
	if (found != _idByName.end()) return found->second;

	int id = static_cast<int>(_vertices.size());
	_vertices.push_back(Vertex{ id, vertexName });
	_adj.emplace_back();        // 인접 리스트의 공간 확장
	_idByName[vertexName] = id; // 이름으로 ID를 찾는 맵에 등록
	return id;
}

// 정점 이름을 사용하여 간선을 추가합니다. 내부적으로 addVertex를 호출해 정점 존재를 보장합니다.
void Graph::addEdge(const std::string& fromName, const std::string& toName, double weight) {
	int from = addVertex(fromName);
	int to = addVertex(toName);
	addEdge(from, to, weight);
}

// 정점 ID를 사용하여 직접 간선을 추가합니다.
void Graph::addEdge(int from, int to, double weight) {
	if (from < 0 || from >= vertexCount() || to < 0 || to >= vertexCount()) {
		throw std::out_of_range("Graph::addEdge(): invalid vertex id");
	}
	_adj[from].push_back(Edge{ from, to, weight });
	// 무방향 그래프인 경우 반대 방향으로도 간선을 추가합니다.
	if (!_directed) {
		_adj[to].push_back(Edge{ to, from, weight });
	}
}

// 특정 이름의 정점이 존재하는지 확인합니다.
bool Graph::hasVertex(const std::string& vertexName) const {
	return _idByName.find(vertexName) != _idByName.end();
}

// 정점 이름으로 고유 ID를 가져옵니다.
int Graph::idOf(const std::string& vertexName) const {
	auto found = _idByName.find(vertexName);
	if (found == _idByName.end()) {
		throw std::invalid_argument("Unknown vertex name: " + vertexName);
	}
	return found->second;
}

// ID를 통해 정점 객체 정보를 가져옵니다.
const Vertex& Graph::vertex(int id) const {
	if (id < 0 || id >= vertexCount()) {
		throw std::out_of_range("Graph::vertex(): invalid vertex id");
	}
	return _vertices[id];
}

// 전체 정점 배열을 반환합니다.
const std::vector<Vertex>& Graph::vertices() const noexcept { return _vertices; }

// 특정 ID 정점의 인접 간선 목록을 반환합니다.
const std::vector<Edge>& Graph::adjacentEdges(int id) const {
	if (id < 0 || id >= vertexCount()) {
		throw std::out_of_range("Graph::adjacentEdges(): invalid vertex id");
	}
	return _adj[id];
}

// 모든 간선 정보를 수집하여 반환합니다.
std::vector<Edge> Graph::edges(bool uniqueUndirectedEdges) const {
	std::vector<Edge> result;
	for (int u = 0; u < vertexCount(); ++u) {
		for (const Edge& e : _adj[u]) {
			// 무방향 그래프에서 중복 간선을 제외할지 결정합니다.
			if (!_directed && uniqueUndirectedEdges && e.from > e.to) continue;
			result.push_back(e);
		}
	}
	return result;
}

// 그래프 연결 정보를 인접 행렬(Adjacency Matrix)로 변환합니다.
std::vector<std::vector<double>> Graph::adjacencyMatrix() const {
	int n = vertexCount();
	std::vector<std::vector<double>> matrix(n, std::vector<double>(n, INF));
	// 대각선 성분(자기 자신으로의 거리)은 0.0으로 초기화합니다.
	for (int i = 0; i < n; ++i) matrix[i][i] = 0.0;
	// 간선 정보를 순회하며 행렬 값을 채웁니다.
	for (int u = 0; u < n; ++u) {
		for (const Edge& e : _adj[u]) {
			matrix[e.from][e.to] = std::min(matrix[e.from][e.to], e.weight);
		}
	}
	return matrix;
}

// 그래프의 인접 리스트를 출력합니다.
void Graph::printAdjacencyList(std::ostream& os) const {
	os << "\nGraph " << _graphName << " (" << vertexCount() << " vertices, " << edgeCount() << " edges)\n";
	for (const Vertex& v : _vertices) {
		os << " " << std::setw(5) << v.name << " : ";
		for (const Edge& e : _adj[v.id]) {
			os << "(" << vertex(e.to).name << ", " << e.weight << ") ";
		}
		os << '\n';
	}
}

// 교수님 예시와 완벽히 동일한 포맷으로 거리 행렬을 출력합니다.
void Graph::printAdjacencyMatrix(std::ostream& os) const {
	auto matrix = adjacencyMatrix();
	// 헤더 출력: 정점 이름들을 일정한 간격(setw(5))으로 정렬
	os << "Distance Matrix of Graph (" << _graphName << ") :\n       |";
	for (const Vertex& v : _vertices) os << std::setw(5) << v.name;
	os << "\n-------+";
	for (int i = 0; i < vertexCount(); ++i) os << "-----";
	os << '\n';

	// 행렬의 각 셀을 출력합니다. 모든 값은 5칸 너비로 정렬하여 표 형식을 유지합니다.
	for (int i = 0; i < vertexCount(); ++i) {
		os << std::setw(6) << vertex(i).name << " |";
		for (int j = 0; j < vertexCount(); ++j) {
			if (matrix[i][j] == INF) os << std::setw(5) << "+oo"; // 도달 불가능은 +oo로 표시
			else os << std::setw(5) << (int)matrix[i][j];
		}
		os << '\n';
	}
}

// 텍스트 파일로부터 그래프 데이터를 읽어와 Graph 객체를 생성합니다.
Graph Graph::fgetGraph(const std::string& fileName, bool directed) {
	std::ifstream fin(fileName);
	if (!fin) throw std::runtime_error("Failed to open graph input file: " + fileName);

	std::string graphName;
	int declaredVertexCount = 0;
	// 파일 첫 줄의 이름과 개수 정보를 읽습니다.
	fin >> graphName >> declaredVertexCount;
	if (!fin) throw std::runtime_error("Invalid graph file header: " + fileName);

	Graph graph(graphName, directed);
	std::string from, to;
	double weight = 0.0;
	// 파일 끝까지 반복하여 간선을 추가합니다.
	while (fin >> from >> to >> weight) graph.addEdge(from, to, weight);
	return graph;
}

// 경로의 정점 ID들을 정점 이름 리스트로 변환합니다.
std::vector<std::string> namesOfPath(const Graph& g, const std::vector<int>& path) {
	std::vector<std::string> result;
	for (int id : path) result.push_back(g.vertex(id).name);
	return result;
}

// 계산된 경로를 "시작 -> 정점 -> 도착" 형태로 출력합니다.
void printPath(std::ostream& os, const Graph& g, const std::vector<int>& path) {
	if (path.empty()) {
		os << "No path";
		return;
	}
	for (std::size_t i = 0; i < path.size(); ++i) {
		if (i > 0) os << " -> ";
		os << g.vertex(path[i]).name;
	}
}