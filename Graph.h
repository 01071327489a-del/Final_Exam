/*
파일명 : Graph.h
작성자 : 조성윤 (22312083)
작성일 : 2026-06-09

주요 기능 설명 :
- 그래프(Graph) 자료구조를 표현하기 위한 핵심 클래스와 관련 구조체들을 정의한 헤더 파일입니다.
- Vertex(정점)와 Edge(간선) 구조체를 기반으로 그래프의 기본 요소를 구성합니다.
- 메모리 효율이 좋은 '인접 리스트(Adjacency List)' 방식을 기반으로 구현되어 있으며,
  알고리즘의 필요에 따라 '인접 행렬(Adjacency Matrix)' 형태로 변환하여 반환할 수도 있습니다.
- 주요 함수 :
  * addVertex, addEdge : 정점의 이름(문자열)이나 ID(정수)를 이용해 그래프에 새로운 요소를 추가합니다.
  * fgetGraph : 외부 텍스트 파일(.txt)로부터 데이터를 읽어와 파싱한 뒤, Graph 객체를 생성해 반환합니다.
  * printAdjacencyList, printAdjacencyMatrix : 그래프 내부의 연결 상태를 터미널 화면에 보기 좋게 출력합니다.
*/

// 헤더 파일이 여러 소스 파일에서 중복 포함되어 발생하는 컴파일 에러를 막기 위한 헤더 가드(Header Guard)입니다.
#ifndef GRAPH_H
#define GRAPH_H

#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <vector>
#include <unordered_map>
#include <limits>
#include <stdexcept>
#include <algorithm>

// 최단 경로 탐색 알고리즘 등에서 사용할 무한대(Infinity) 값을 constexpr로 컴파일 타임 상수로 정의합니다.
constexpr double INF = std::numeric_limits<double>::infinity();

/* Vertex 구조체 정의
   그래프의 노드(정점)를 나타냅니다.
   배열의 인덱스로 활용할 고유 정수 id와 화면에 출력될 문자열 name을 멤버로 가집니다.
*/
struct Vertex {
	int id{};
	std::string name;
};

/* Edge 구조체 정의
   그래프의 선(간선)을 나타냅니다.
   출발지(from)와 도착지(to)의 정점 id를 가지고 있으며, 경로 탐색에 쓰일 가중치(weight)를 포함합니다.
*/
struct Edge {
	int from{};
	int to{};
	double weight{};
};

/* Vertex 객체를 std::cout 등으로 직관적으로 출력할 수 있도록 << 연산자를 오버로딩합니다. */
inline std::ostream& operator<<(std::ostream& os, const Vertex& v) {
	return os << v.name;
}

/* Edge 객체를 출력할 때 "Edge(0 -> 1, w=1.5)" 형태로 포맷팅되도록 << 연산자를 오버로딩합니다. */
inline std::ostream& operator<<(std::ostream& os, const Edge& e) {
	return os << "Edge(" << e.from << " -> " << e.to << ", w=" << e.weight << ")";
}

/* Graph 클래스 정의
   그래프의 전체적인 상태를 저장하고, 요소 추가/탐색 및 파일 입출력을 관리하는 메인 클래스입니다.
*/
class Graph {
private:
	std::string _graphName; // 그래프의 이름 (예: "Korea", "Simplified_USA")
	bool _directed{};       // 단방향(Directed) 그래프인지, 양방향(Undirected) 그래프인지 여부

	std::vector<Vertex> _vertices;          // 그래프에 존재하는 모든 정점(Vertex) 객체들을 저장하는 배열
	std::vector<std::vector<Edge>> _adj;    // 각 정점마다 연결된 간선들의 목록을 저장하는 인접 리스트(Adjacency List)

	// 정점의 이름(문자열)을 입력하면 해당 정점의 고유 id(정수)를 즉시(O(1)) 찾을 수 있게 해주는 해시맵입니다.
	std::unordered_map<std::string, int> _idByName;

public:
	// 그래프 객체 생성자. 의도치 않은 암시적 형변환을 막기 위해 explicit 키워드를 사용합니다.
	explicit Graph(std::string name = "", bool directed = false);

	// 그래프의 기본 속성값들을 반환하는 Getter 함수들입니다.
	// 함수 내부에서 예외를 던지지 않음을 보장하기 위해 noexcept를 붙였습니다.
	const std::string& name() const noexcept;
	bool isDirected() const noexcept;
	int vertexCount() const noexcept;
	int edgeCount() const noexcept;

	// 정점을 새롭게 추가하고, 추가된 정점의 부여된 id를 반환합니다.
	int addVertex(const std::string& vertexName);

	// 출발지와 도착지의 "이름(문자열)" 또는 "고유 id(정수)"를 이용해 새로운 간선을 추가하는 오버로딩 함수들입니다.
	void addEdge(const std::string& fromName, const std::string& toName, double weight = 1.0);
	void addEdge(int from, int to, double weight = 1.0);

	// 특정 이름의 정점이 그래프 내에 존재하는지(해시맵 이용) 확인합니다.
	bool hasVertex(const std::string& vertexName) const;

	// 특정 정점 이름에 해당하는 고유 id 값을 반환합니다. (없을 경우 예외 발생)
	int idOf(const std::string& vertexName) const;

	// 입력받은 정수 id에 해당하는 Vertex 객체의 레퍼런스를 반환합니다.
	const Vertex& vertex(int id) const;

	// 그래프의 전체 정점 목록과, 특정 정점에 연결된 간선 목록을 반환합니다.
	const std::vector<Vertex>& vertices() const noexcept;
	const std::vector<Edge>& adjacentEdges(int id) const;

	// 그래프에 존재하는 모든 간선의 목록을 하나의 1차원 배열로 쭉 모아서 반환합니다.
	// 무방향 그래프일 경우 양방향으로 등록된 중복 간선을 하나만 포함할지 선택할 수 있습니다.
	std::vector<Edge> edges(bool uniqueUndirectedEdges = true) const;

	// 현재 '인접 리스트' 형태로 저장된 그래프 연결 정보를 '인접 행렬(2차원 배열)' 형태로 변환하여 반환합니다.
	// 플로이드-워셜 같이 행렬 형태가 필요한 알고리즘에서 주로 호출합니다.
	std::vector<std::vector<double>> adjacencyMatrix() const;

	// 그래프의 연결 상태를 터미널 화면에 보기 좋게 출력해주는 디버깅 및 시각화용 함수들입니다.
	void printAdjacencyList(std::ostream& os) const;
	void printAdjacencyMatrix(std::ostream& os) const;

	// 정적(static) 메서드. 클래스 객체 생성 없이 Graph::fgetGraph(...) 형태로 호출합니다.
	// 지정된 이름의 텍스트 파일을 열고 파싱하여 완성된 Graph 객체를 찍어내는 팩토리(Factory) 역할을 합니다.
	static Graph fgetGraph(const std::string& fileName, bool directed = false);
};

/* 헬퍼(Helper) 편의성 함수들
   탐색 알고리즘이 뱉어낸 정수 id 기반의 경로 배열(path)을 다루기 쉽게 변환해 줍니다.
*/

// 경로 배열(정수 id들의 모임)을 받아서, 실제 사용자가 알아볼 수 있는 정점 이름(문자열) 배열로 변환합니다.
std::vector<std::string> namesOfPath(const Graph& g, const std::vector<int>& path);

// 완성된 경로를 화면에 "A -> B -> C" 형식으로 예쁘게 포맷팅하여 출력해 줍니다.
void printPath(std::ostream& os, const Graph& g, const std::vector<int>& path);

#endif