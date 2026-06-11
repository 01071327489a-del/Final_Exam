/*
파일명 : Practice_13.1.cpp
작성자 : 조성윤 (22312083)
작성일 : 2026-06-09

주요 기능 설명 :
- 그래프 파일(Korea_11.txt)을 자동으로 불러와 그래프(Graph) 객체를 생성합니다.
- 사용자로부터 원하는 탐색 알고리즘 번호를 입력받아 동적으로 실행합니다.
- 깊이 우선 탐색(DFS), 너비 우선 탐색(BFS), 다익스트라(Dijkstra) 최단 경로 탐색,
  프림(Prim) 및 크루스칼(Kruskal) 최소 신장 트리(MST) 알고리즘의 동작을 테스트합니다.
*/
#include "Graph.h"
#include "GraphAlgorithms.h" 
#include <iostream>
#include <string>

using namespace std;

int main() {
	// 그래프 데이터를 저장하고 관리할 객체 생성
	Graph graph;

	/* 1. 파일에서 그래프 데이터 불러오기 */
	// 프로그램 시작 시 사용자 입력 없이 Korea_11.txt 데이터를 즉시 로드합니다.
	// 파일이 없거나 데이터 형식이 잘못되었을 경우를 대비해 예외 처리(try-catch)를 사용합니다.
	try {
		graph = Graph::fgetGraph("Korea_11.txt", false);
	}
	catch (const exception& ex) {
		// 파일 로드 실패 시 에러 메시지 출력 후 비정상 종료(1 반환)
		cout << "그래프 파일을 불러오는 데 실패했습니다: " << ex.what() << '\n';
		return 1;
	}

	/* 2. 그래프 인접 행렬 출력 */
	// 파일에서 읽어온 그래프 구조가 정상적인지 확인하기 위해 인접 행렬(Adjacency Matrix) 형태로 화면에 출력합니다.
	graph.printAdjacencyMatrix(cout);
	cout << endl;

	/* 3. 테스트할 알고리즘 모드 입력받기 */
	// 실행할 탐색 알고리즘 번호를 사용자로부터 1회 입력받습니다.
	cout << "Input test mode (1: Depth_First_Search(DFS), 2: Breadth_First_Search(BFS), 3: Dijkstra_Shortest_Path_Search(),\n"
		<< "    4: test_MST_PrimJarnik, 5: test_MST_Kruskal, 0 : quit) : ";
	int testMode = 0;
	cin >> testMode;
	cout << endl;

	// 0을 입력하면 더 이상 진행하지 않고 프로그램을 즉시 정상 종료합니다.
	if (testMode == 0) return 0;

	// 탐색의 출발점과 도착점을 저장할 문자열 변수 선언
	string startName, targetName;

	// 선택한 모드에 따라 필요한 정점(Vertex)의 이름을 입력받습니다.
	if (testMode >= 1 && testMode <= 3) {
		// 1(DFS), 2(BFS), 3(다익스트라) 모드는 출발점과 도착점이 모두 필요합니다.
		if (testMode == 3) {
			cout << "Testing Breadth First Search with Dijkstra Algorithm\n";
			cout << "Input start and end vertex names of path to search shortest path ( . . to quit) : ";
		}
		else {
			cout << "Input start and target vertex names: ";
		}

		cin >> startName >> targetName;

		// 사용자가 "."을 입력하면 탐색을 취소하고 프로그램을 종료합니다.
		if (startName == "." || targetName == ".") return 0;
	}
	else if (testMode == 4) {
		// 4번 프림 알고리즘(Prim MST)은 시작점(Start node) 하나만 주어지면 인접 간선을 통해 트리를 확장할 수 있습니다.
		cout << "Input start vertex name of minimum spanning tree : ";
		cin >> startName;
	}
	// 5번 크루스칼 알고리즘은 전체 간선을 대상으로 가중치를 정렬해 트리를 구성하므로 별도의 정점 입력이 필요 없습니다.

	/* 4. 선택한 알고리즘 실행 및 결과 출력 */
	// 입력받은 testMode 번호에 맞춰 해당되는 GraphAlgorithms 클래스의 정적(static) 메서드를 호출합니다.
	switch (testMode) {
	case 1: {
		// 깊이 우선 탐색 (DFS) 실행
		PathResult result = GraphAlgorithms::dfsPath(graph, startName, targetName);
		if (result.found) {
			cout << "DFS path found with distance " << ": ";
			printPath(cout, graph, result.path); // 경로를 "A -> B -> C" 형태로 이어서 출력
			cout << " / cost  = " << result.distance << endl;
		}
		else cout << "DFS path not found.\n";
		cout << endl;
		break;
	}
	case 2: {
		// 너비 우선 탐색 (BFS) 실행
		PathResult result = GraphAlgorithms::bfsPath(graph, startName, targetName);
		if (result.found) {
			cout << "BFS path found with distance " << ": ";
			printPath(cout, graph, result.path);
			cout << " / cost  = " << result.distance << endl;
		}
		else cout << "BFS path not found.\n";
		cout << endl;
		break;
	}
	case 3: {
		// 다익스트라(Dijkstra) 최단 경로 탐색 실행
		// 무방향/방향 그래프 여부와 가중치에 따라 왕복 경로가 다를 수 있으므로 정방향과 역방향을 모두 탐색하여 출력합니다.

		/* 3-1. 정방향 탐색 (start -> target) */
		PathResult result1 = GraphAlgorithms::dijkstraPath(graph, startName, targetName);
		if (result1.found) {
			cout << "reached to the target node (" << targetName << ") at Least Cost = " << result1.distance << endl;
			cout << "Path found by DijkstraShortestPath from " << startName << " to " << targetName << " : "
				<< "Path found (" << startName << " => " << targetName << ") : ";
			printPath(cout, graph, result1.path);
			cout << endl;
		}
		else cout << "Dijkstra path not found.\n";

		/* 3-2. 역방향 탐색 (target -> start) */
		PathResult result2 = GraphAlgorithms::dijkstraPath(graph, targetName, startName);
		if (result2.found) {
			cout << "reached to the target node (" << startName << ") at Least Cost = " << result2.distance << endl;
			cout << "Path found by DijkstraShortestPath from " << targetName << " to " << startName << " : "
				<< "Path found (" << targetName << " => " << startName << ") : ";
			printPath(cout, graph, result2.path);
			cout << endl;
		}
		else cout << "Dijkstra path not found.\n";
		cout << endl;
		break;
	}
	case 4: {
		// 프림(Prim) 알고리즘을 이용한 최소 신장 트리(MST) 구성
		MSTResult result = GraphAlgorithms::primMST(graph, startName);
		if (result.connected) {
			cout << "Selected edges:\n";
			// MST에 포함된 모든 간선(Edge)들의 출발점, 도착점, 가중치를 포맷에 맞춰 순서대로 출력합니다.
			for (const Edge& e : result.edges) {
				cout << "Edge(" << std::setw(3) << graph.vertex(e.from).name << " -> "
					<< std::setw(3) << graph.vertex(e.to).name << " :"
					<< std::setw(8) << e.weight << "), ";
			}
			cout << "\n\n";
		}
		else cout << "Prim MST not found.\n";
		break;
	}
	case 5: {
		// 크루스칼(Kruskal) 알고리즘을 이용한 최소 신장 트리(MST) 구성
		MSTResult result = GraphAlgorithms::kruskalMST(graph);
		if (result.connected) {
			cout << "Selected edges:\n";
			// 크루스칼은 전체 간선을 기준으로 가장 가중치가 적은 것부터 병합(Union-Find)하므로 선택된 간선을 그대로 출력합니다.
			for (const Edge& e : result.edges) {
				cout << std::left << std::setw(3) << graph.vertex(e.from).name << " --  " << std::setw(3) << graph.vertex(e.to).name << "  weight=" << e.weight << '\n';
			}
			// 연결된 모든 간선의 가중치 합을 구해 트리의 총비용(Total weight)을 출력합니다.
			cout << "Total weight = " << result.totalWeight << "\n\n";
		}
		else cout << "Kruskal MST not found.\n";
		break;
	}
	default: break;
	}

	// 선택한 1개의 알고리즘 동작이 완료되면 더 이상 루프를 돌지 않고 0을 반환하며 깔끔하게 프로그램을 종료합니다.
	return 0;
}