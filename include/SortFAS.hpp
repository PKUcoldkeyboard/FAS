#ifndef __SORT_FAS_HPP__
#define __SORT_FAS_HPP__
#include "strategy.h"
#include "spdlog_common.h"
#include "utils.hpp"
#include <chrono>

// SortGraph数据结构，使用链式前向星
class SortGraph {
public:
    int n;
    struct Edge {
        int to;
        int next;
    };
    SortGraph(int n) : n(n) {
        head.resize(n, -1);
    }
    std::vector<Edge> edges;
    std::vector<int> head;

    void add_edge(int u, int v) {
        edges.emplace_back(Edge{v, head[u]});
        head[u] = edges.size() - 1;
    }
};

class SortFAS : public FASStrategy {
public:
    std::vector<std::pair<int, int>> getFeedbackArcSet(const std::string &input) {
        SPDLOG_INFO("Starting SortFAS...");
        std::vector<std::pair<int, int>> edges;
        int n = 0;
        read_edges(input, edges, n);
        SortGraph g(n);
        construct_graph(edges, g);

        // 初始化状态
        std::vector<int> arrangement(n);
        std::vector<std::pair<int, int>> fas;
        std::iota(arrangement.begin(), arrangement.end(), 0);
        std::vector<std::pair<int, int>>().swap(edges);

        // 开始算法
        auto start = std::chrono::high_resolution_clock::now();

        for (int i = 0; i < n; i++) {
            int val = 0, min_val = 0, loc = i;
            int v = arrangement[i];
            if (g.head[v] == -1) {
                continue;
            }
            for (int j = loc - 1; j >= 0; j--) {
                int w = arrangement[j];
                if (g.head[w] == -1) {
                    continue;
                }
                bool e_vw = false, e_wv = false;
                for (int k = g.head[v]; k != -1; k = g.edges[k].next) {
                    if (g.edges[k].to == w) {
                        e_vw = true;
                        break;
                    }
                }
                for (int k = g.head[w]; k != -1; k = g.edges[k].next) {
                    if (g.edges[k].to == v) {
                        e_wv = true;
                        break;
                    }
                }
                if (e_vw) {
                    val -= 1;
                } else if (e_wv) {
                    val += 1;
                }

                if (val <= min_val) {
                    min_val = val;
                    loc = j;
                }
            }
            // Insert v at position loc
            arrangement.erase(arrangement.begin() + i);
            arrangement.insert(arrangement.begin() + loc, v);
        }

        int size = arrangement.size();
        for (int i = 0; i < size - 1; i++) {
            int u = arrangement[i];
            if (g.head[u] == -1) {
                continue;
            }
            for (int j = i + 1; j < size; j++) {
                int v = arrangement[j];
                if (g.head[v] == -1) {
                    continue;
                }
                for (int k = g.head[v]; k != -1; k = g.edges[k].next) {
                    if (g.edges[k].to == u) {
                        fas.emplace_back(v, u);
                    }
                }
            }
        }
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        SPDLOG_INFO("Successfully compute FAS, FAS number: {}, Time Elapsed: {}s", fas.size(), duration / 1000.0);

        return fas;
    }
private:
    void construct_graph(const std::vector<std::pair<int, int>> &edges, SortGraph &g) {
        for (const auto &edge : edges) {
            g.add_edge(edge.first, edge.second);
        }
    }
};

#endif // __SORT_FAS_HPP__