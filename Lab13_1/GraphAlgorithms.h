/*
파일명 : GraphAlgorithms.h
작성자 : 조성윤 (22312083)
작성일 : 2026-06-09

주요 기능 설명 :
- 그래프 탐색 및 최단 경로 알고리즘(DFS, BFS, Dijkstra, Floyd-Warshall)과
  최소 신장 트리(MST) 알고리즘(Kruskal, Prim)을 모아둔 GraphAlgorithms 클래스의 선언부입니다.
- 각 알고리즘의 결괏값을 깔끔하게 반환하고 관리하기 위해 PathResult와 MSTResult 구조체를 정의합니다.
*/

// 헤더 파일의 중복 포함(컴파일 에러)을 방지하기 위한 헤더 가드입니다.
#ifndef GRAPH_ALGORITHMS_H
#define GRAPH_ALGORITHMS_H

#include "Graph.h"
#include <queue>
#include <stack>
#include <functional>
#include <numeric>

/* 경로 탐색 알고리즘(DFS, BFS, 다익스트라)의 실행 결과를 담는 구조체입니다.
 여러 개의 반환값을 하나의 객체로 묶어서 넘겨주기 위해 사용합니다.
*/
struct PathResult {
	bool found{};            // 목적지까지의 경로를 찾았는지 여부 (true/false)
	double distance{ INF };  // 시작점에서 목적지까지의 총 거리(비용). 초기값은 무한대(INF)입니다.
	std::vector<int> path;   // 시작점부터 도착점까지 거쳐간 정점(Vertex)들의 ID가 순서대로 담긴 배열입니다.
};

/* 최소 신장 트리(MST) 알고리즘(크루스칼, 프림)의 실행 결과를 담는 구조체입니다.
*/
struct MSTResult {
	bool connected{};           // 그래프의 모든 노드가 끊어짐 없이 성공적으로 연결되었는지 여부
	double totalWeight{};       // MST를 구성하는 모든 간선(Edge)들의 가중치 총합
	std::vector<Edge> edges;    // MST에 최종적으로 선택되어 포함된 간선들의 목록입니다.
};

/* 다양한 그래프 알고리즘을 제공하는 클래스입니다.
 상태를 가질 필요가 없으므로 객체 생성 없이 클래스 이름으로 바로 호출할 수 있도록
 모든 함수를 정적(static) 메서드로 선언했습니다. (예: GraphAlgorithms::dfsPath(...))
*/
class GraphAlgorithms {
public:
	// 깊이 우선 탐색(DFS)을 사용하여 두 노드 간의 경로를 찾습니다.
	static PathResult dfsPath(const Graph& graph, const std::string& startName, const std::string& targetName);

	// 너비 우선 탐색(BFS)을 사용하여 두 노드 간의 경로를 찾습니다. (가중치가 없을 때 최단 경로 보장)
	static PathResult bfsPath(const Graph& graph, const std::string& startName, const std::string& targetName);

	// 다익스트라(Dijkstra) 알고리즘을 사용하여 가중치 그래프에서 최단 거리와 경로를 찾습니다.
	static PathResult dijkstraPath(const Graph& graph, const std::string& startName, const std::string& targetName);

	// 플로이드-워셜(Floyd-Warshall) 알고리즘으로 모든 정점 쌍 간의 최단 거리를 계산하여 2차원 행렬로 반환합니다.
	static std::vector<std::vector<double>> floydWarshall(const Graph& graph);

	// 크루스칼(Kruskal) 알고리즘을 사용하여 간선들을 가중치 오름차순으로 정렬한 뒤 최소 신장 트리를 만듭니다.
	static MSTResult kruskalMST(const Graph& graph);

	// 프림(Prim) 알고리즘을 사용하여 특정 시작점에서부터 출발해 인접한 가장 가벼운 간선을 추가하며 최소 신장 트리를 만듭니다.
	static MSTResult primMST(const Graph& graph, const std::string& startName);

private:
	/* 탐색이 완료된 후, 각 노드가 어디서부터 왔는지 기록해둔 배열(previous)을 역추적하여
	 실제 시작점부터 도착점까지의 순차적인 경로 배열(vector)을 만들어주는 내부 도우미(Helper) 함수입니다.
	*/
	static std::vector<int> buildPath(int start, int target, const std::vector<int>& previous);
};

#endif