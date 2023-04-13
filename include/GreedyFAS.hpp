#ifndef __GREEDY_FAS_HPP__
#define __GREEDY_FAS_HPP__
#include "strategy.h"
#include "spdlog_common.h"
#include "utils.hpp"
#include <chrono>
#include <numeric>

// 定义GreedyGraph数据类型，用链式前向星
class GreedyGraph {
public:
    int n;
    struct Edge {
        int to;
        int next;
    };
    GreedyGraph(int n) : n(n) {
        head.resize(n, -1);
        out_degree.resize(n, 0);
    }
    std::vector<Edge> edges;
    std::vector<int> head;
    std::vector<int> out_degree;

    void add_edge(int u, int v) {
        edges.emplace_back(Edge{v, head[u]});
        head[u] = edges.size() - 1;
        out_degree[u]++;
    }
};

class GreedyFAS : public FASStrategy {
public:
    std::vector<std::pair<int, int>> getFeedbackArcSet(const std::string &input) override {
        SPDLOG_INFO("Starting GreedyFAS...");
        std::vector<std::pair<int, int>> edges;
        int n = 0;
        read_edges(input, edges, n);
        
        GreedyGraph g(n), inverse(n);
        construct_graph(edges, g, inverse);
        std::vector<std::pair<int, int>>().swap(edges);

        // 初始化状态
        std::vector<int> state(n, 0);
        std::vector<int> s1, s2;
        std::vector<std::pair<int, int>> fas;

        // 执行算法
        auto start = std::chrono::high_resolution_clock::now();
        while (std::accumulate(state.begin(), state.end(), 0) != n) {
            while (true) {
                std::vector<int> sink;
                get_sink(g, sink, state);
                if (sink.empty()) {
                    break;
                }
                const auto &v = sink[0];
                state[v] = 1;
                s2.insert(s2.begin(), v);
                update_degree(g, inverse, v);
            }
            while (true) {
                std::vector<int> source;
                get_source(inverse, source, state);
                if (source.empty()) {
                    break;
                }
                const auto &u = source[0];
                state[u] = 1;
                s1.emplace_back(u);
                update_degree(g, inverse, u);
            }
            if (std::accumulate(state.begin(), state.end(), 0) != n) {
                int max_delta = std::numeric_limits<int>::min();
                int u = -1;
                for (int i = 0; i < n; i++) {
                    if (g.head[i] == -1) {
                        continue;
                    }
                    if (state[i] != 0) {
                        continue;
                    }
                    int delta = g.out_degree[i] - inverse.out_degree[i];
                    if (delta > max_delta) {
                        max_delta = delta;
                        u = i;
                    }
                }
                if (u != -1) {
                    s1.push_back(u);
                    state[u] = 1;
                    update_degree(g, inverse, {u});
                }
            }
        }

        s1.insert(s1.end(), s2.begin(), s2.end());

        int size = s1.size();

        for (int i = 0; i < size - 1; i++) {
            int u = s1[i];
            if (g.head[u] == -1) {
                continue;
            }
            for (int j = i + 1; j < size; j++) {
                int v = s1[j];
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
    void construct_graph(const std::vector<std::pair<int, int>> &edges, GreedyGraph &g, GreedyGraph &inverse) {
        for (const auto &edge : edges) {
            g.add_edge(edge.first, edge.second);
            inverse.add_edge(edge.second, edge.first);
        }
    }

    void update_degree(GreedyGraph &g, GreedyGraph &inverse, const int &u) {
        // 出边的入度减1
        for (int i = g.head[u]; i != -1; i = g.edges[i].next) {
            int v = g.edges[i].to;
            inverse.out_degree[v]--;
        }
        // 入边的出度减1
        for (int i = inverse.head[u]; i != -1; i = inverse.edges[i].next) {
            int v = inverse.edges[i].to;
            g.out_degree[v]--;
        }
    }
    void get_sink(GreedyGraph &g, std::vector<int> &sink, const std::vector<int> &state) {
        for (int i = 0; i < g.n; i++) {
            if (g.head[i] == -1) {
                continue;
            }
            if (state[i] != 0) {
                continue;
            }
            if (g.out_degree[i] == 0) {
                sink.emplace_back(i);
            }
        }
    }
    void get_source(GreedyGraph &inverse, std::vector<int> &source, const std::vector<int> &state) {
        for (int i = 0; i < inverse.n; i++) {
            if (inverse.head[i] == -1) {
                continue;
            }
            if (state[i] != 0) {
                continue;
            }
            if (inverse.out_degree[i] == 0) {
                source.emplace_back(i);
            }
        }
    }
};

#endif // __GREEDY_FAS_HPP__