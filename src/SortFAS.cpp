#include "SortFAS.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cstdio>
#include <chrono>

boost::container::vector<EdgePair> getBackArc(emhash7::HashMap<int, boost::container::vector<int>>& graph, boost::container::vector<int>& index2node, boost::container::vector<int>& node2index) {
    // 从排列结果index2node和node2index中获取反向弧
    boost::container::vector<EdgePair> results;
    for (int i = 0; i < index2node.size(); i++) {
        int v = index2node[i];
        if (graph.find(v) != graph.end()) {
            for (int k = 0; k < graph[v].size(); k++) {
                int w = graph[v][k];
                if (node2index[w] < i) {
                    results.emplace_back(v, w);
                }
            }
        }
    }
    return results;
};


boost::container::vector<EdgePair> sorting(emhash7::HashMap<int, boost::container::vector<int>>& graph, int n) {
    // sortFAS算法主体，返回结果是反馈弧集合
    boost::container::vector<int> node2index;
    boost::container::vector<int> index2node;
    for (int i = 0; i <= n; i++) {
        node2index.push_back(i);
        index2node.push_back(i);
    }
    for (int v = n; v >= 0; v--) {
        int val = 0;
        int minVal = 0;
        int fromIndex = node2index[v];
        int toIndex = fromIndex;
        for (int j = fromIndex - 1; j >= 0; j--) {
            int w = index2node[j];
            bool foundW = false, foundV = false;
            if (graph.find(v) != graph.end()) {
                for (int k = 0; k < graph[v].size(); k++) {
                    if (graph[v][k] == w) {
                        foundW = true;
                        break;
                    }
                }
            }
            if (graph.find(w) != graph.end()) {
                for (int k = 0; k < graph[w].size(); k++) {
                    if (graph[w][k] == v) {
                        foundV = true;
                        break;
                    }
                }
            }
            if (foundW) {
                val -= 1;
            }
            if (foundV) {
                val += 1;
            }
            if (val <= minVal) {
                toIndex = j;
                minVal = val;
            }
        }
        for (int i = fromIndex; i > toIndex; i--) {
            int adjustedNode = index2node[i - 1];
            node2index[adjustedNode] += 1;
            index2node[i] = adjustedNode;
        }
        index2node[toIndex] = v;
        node2index[v] = toIndex;
    }

    return getBackArc(graph, index2node, node2index);
}


emhash7::HashMap<int, boost::container::vector<int>> constructGraph(Graph &g, int &n){
    emhash7::HashMap<int, boost::container::vector<int>> adjList; // 图的邻接链表
    for (const auto &e : boost::make_iterator_range(boost::edges(g))) {
        int v = boost::source(e, g);
        int w = boost::target(e, g);
        boost::container::vector<int> adj;
        if (adjList.find(v) == adjList.end()) {
            adjList[v] = adj;
        } else {
            adj = adjList[v];
        }
        adj.push_back(w);
        n = std::max(n, w);
        n = std::max(n, v);
        adjList[v] = adj;
    }

    return adjList;
}


boost::container::vector<EdgePair> SortFAS::getFeedbackArcSet(Graph &g) {
    SPDLOG_INFO("Starting SortFAS...");
    int n = 0;
    emhash7::HashMap<int, boost::container::vector<int>> graph = constructGraph(g, n);
    auto start = std::chrono::high_resolution_clock::now();
    auto result = sorting(graph, n);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    SPDLOG_INFO("Successfully compute FAS, FAS number: {}, Time Elapsed: {} s", result.size(), duration.count() / 1000.0);
    return result;
}