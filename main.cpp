/*
 * File   : main.cpp
 * Author : Cho-Seong-Yun (Student-ID: 22312083)
 * Date   : 2026.06.05
 * [Program] C++ STL ordered map과 unordered map의 삽입(insert) 및
 * 탐색(find) 성능(실행 시간) 측정 및 비교 프로그램
**/
#include <iostream>
#include <string>
#include <vector>
#include <map>
#include <unordered_map>
#include <chrono> // 시간 측정 로직
#include <random>
#include <iomanip> // 출력 포맷팅(setw)을 위해 포함

using namespace std;
using namespace std::chrono;
using Clock = chrono::high_resolution_clock;

// 테스트할 데이터의 총 개수와 탐색(Find) 테스트 횟수 상수 정의 (C++14 숫자 구분자 사용)
constexpr int NUM_DATA = 1'000'000;
constexpr int NUM_FIND_TEST = 100'000;

// 템플릿 함수: 컨테이너의 데이터 삽입 성능(시간)을 마이크로초(us) 단위로 측정
template <typename Container>
long long measureInsertPerformance(Container& container, const vector<pair<int, string>>& kvStr_pairs) {
    auto start = Clock::now();
    
    // 준비된 벡터의 모든 (key, value) 쌍을 컨테이너에 삽입
    for (const auto& kv : kvStr_pairs) {
        container.insert(kv);
    }

    auto end = Clock::now();
    // 마이크로초(us) 단위로 걸린 시간 반환
    return duration_cast<microseconds>(end - start).count();
}

// 템플릿 함수: 컨테이너의 데이터 탐색(Find) 성능(시간)을 마이크로초(us) 단위로 측정
template <typename Container>
long long measureFindPerformance(const Container& container, const vector<int>& testKeys) {
    auto start = Clock::now();

    // 컨테이너에서 데이터 탐색
    for (int key : testKeys) {
        container.find(key);
    }

    auto end = Clock::now();
    return duration_cast<microseconds>(end - start).count();
}

int main() {
    // 프로그램 시작 안내 문구 출력
    cout << "Comparison of performance of insert() and find() in std::map and std::unordered_map\n";
    cout << "Total num_data = " << NUM_DATA << ", num_find_test = " << NUM_FIND_TEST << "\n\n";


    map<int, string> orderedMap;
    unordered_map<int, string> unorderedMap;
    unorderedMap.max_load_factor(0.75);
    unorderedMap.reserve(NUM_DATA);

    cout << "[1] kv_pairs(stID, \"Student_\" + to_string(stID)) creation and insertions ...\n";
    vector<pair<int, string>> kv_pairs;
    kv_pairs.reserve(NUM_DATA);

  
    for (int stID = 0; stID < NUM_DATA; ++stID) {
        kv_pairs.push_back({ stID, "Student_" + to_string(stID) });
    }
    // 2. 해시 구조 (std::unordered_multimap) 삽입 시간 측정
  // 해시 테이블은 충돌이 적을 경우 평균 O(1)의 속도로 빠르게 삽입됨
    long long orderedInsertTime, unorderedInsertTime;
    orderedInsertTime = measureInsertPerformance(orderedMap, kv_pairs);
    unorderedInsertTime = measureInsertPerformance(unorderedMap, kv_pairs);

    // insert 성능 측정 결과 출력 (결과값 간격 정렬)
    cout << "std::map insert time       : " << setw(8) << right << orderedInsertTime << " [us]\n";
    cout << "std::unordered_map insert time: " << setw(8) << right << unorderedInsertTime << " [us]\n";

    // 탐색 성능 테스트를 위한 난수 기반의 키 10만 개 생성
    vector<int> testKeys;
    testKeys.reserve(NUM_FIND_TEST);
    mt19937 gen(12345); 
    uniform_int_distribution<int> dist(0, NUM_DATA - 1);

    // testKeys 생성
    for (int i = 0; i < NUM_FIND_TEST; ++i) {
        testKeys.push_back(dist(gen));
    }

    cout << "\n[2] Performance measurement of find() for " << NUM_FIND_TEST << " test keys\n";
    long long orderedFindTime, unorderedFindTime;
    orderedFindTime = measureFindPerformance(orderedMap, testKeys);
    unorderedFindTime = measureFindPerformance(unorderedMap, testKeys);

    // find 성능 측정 결과 출력 (결과값 간격 정렬)
    cout << "std::map find time         : " << setw(8) << right << orderedFindTime << " [us]\n";
    cout << "std::unordered_map find time: " << setw(8) << right << unorderedFindTime << " [us]\n";

    return 0;
}