#include "SortFAS.h"

boost::container::vector<int> sorting(boost::unordered_map<int, boost::container::vector<int>>& graph, int n) {
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

    return index2node;
}

boost::container::vector<EdgePair> SortFAS::getFeedbackArcSet(Graph &g) {
    boost::container::vector<EdgePair> feedback_arcs;
    SPDLOG_INFO("Starting SortFAS...");
    
    boost::unordered_map<int, boost::container::vector<int>> adjList;
    for (auto vi : boost::make_iterator_range(boost::vertices(g))) {
        for (auto ei : boost::make_iterator_range(boost::out_edges(vi, g))) {
            int from = boost::source(ei, g);
            int to = boost::target(ei, g);
            if (adjList.find(from) == adjList.end()) {
                boost::container::vector<int> vec;
                vec.push_back(to);
                adjList[from] = vec;
            } else {
                adjList[from].push_back(to);
            }
        }
    }
    auto n = boost::num_vertices(g);
    auto result = sorting(adjList, n);

    boost::container::vector<int> index_to_order(n, -1);

    for (int i = 0; i < n; i++) {
        index_to_order[result[i]] = i;
    }

    for (int u = 0; u < n; u++) {
        // 遍历u的出边
        for (auto ei : boost::make_iterator_range(boost::out_edges(u, g))) {
            int v = boost::target(ei, g);
            if (index_to_order[u] > index_to_order[v]) {
                feedback_arcs.emplace_back(std::make_pair(g[u], g[v]));
            }
        }
    }

    return feedback_arcs;
}