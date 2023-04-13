#include "GreedyFAS.h"
#include <boost/graph/graph_utility.hpp>
#include <numeric>
#include <random>

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
    boost::container::vector<Edge> edges;
    boost::container::vector<int> head;
    boost::container::vector<int> out_degree;

    void add_edge(int u, int v) {
        edges.emplace_back(Edge{v, head[u]});
        head[u] = edges.size() - 1;
        out_degree[u]++;
    }
};

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

void get_sink(GreedyGraph &g, boost::container::vector<int> &sink, const boost::container::vector<int> &state) {
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

void get_source(GreedyGraph &inverse, boost::container::vector<int> &source, const boost::container::vector<int> &state) {
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

boost::container::vector<EdgePair> GreedyFAS::getFeedbackArcSet(Graph &g) {
    SPDLOG_INFO("Starting GreedyFAS...");

    // 重新存图
    int n = 0;
    // n等于g[v]的最大值
    for (const auto &v : boost::make_iterator_range(boost::vertices(g))) {
        n = std::max(n, g[v]);
    }
    n++;
    GreedyGraph graph(n), inverse(n);
    for (const auto &e : boost::make_iterator_range(boost::edges(g))) {
        int u = boost::source(e, g);
        int v = boost::target(e, g);
        graph.add_edge(g[u], g[v]);
        inverse.add_edge(g[v], g[u]);
    }
    boost::container::vector<EdgePair> result;
    auto start = std::chrono::high_resolution_clock::now();

    // 初始化状态
    boost::container::vector<int> state(n + 1, 0);
    boost::container::vector<int> s1, s2;

    while (std::accumulate(state.begin(), state.end(), 0) != n) {
        while (true) {
            boost::container::vector<int> sink;
            get_sink(graph, sink, state);
            if (sink.empty()) {
                break;
            }
            srand(time(nullptr));
            int idx = rand() % sink.size();
            const auto &v = sink[idx];
            state[v] = 1;
            s2.insert(s2.begin(), v);
            update_degree(graph, inverse, v);
        }
        while (true) {
            boost::container::vector<int> source;
            get_source(inverse, source, state);
            if (source.empty()) {
                break;
            }
            srand(time(nullptr));
            int idx = rand() % source.size();
            const auto &u = source[idx];
            state[u] = 1;
            s1.emplace_back(u);
            update_degree(graph, inverse, u);
        }
        if (std::accumulate(state.begin(), state.end(), 0) != n) {
            int max_delta = std::numeric_limits<int>::min();
            int u = -1;
            for (int i = 0; i < n; i++) {
                if (state[i] != 0) {
                    continue;
                }
                int delta = graph.out_degree[i] - inverse.out_degree[i];
                if (delta > max_delta) {
                    max_delta = delta;
                    u = i;
                }
            }
            if (u != -1) {
                s1.push_back(u);
                state[u] = 1;
                update_degree(graph, inverse, {u});
            }
        }
    }
    s1.insert(s1.end(), s2.begin(), s2.end());

    for (int i = 0; i < s1.size() - 1; i++) {
        int u = s1[i];
        for (int j = i + 1; j < s1.size(); j++) {
            int v = s1[j];
            for (int k = graph.head[v]; k != -1; k = graph.edges[k].next) {
                if (graph.edges[k].to == u) {
                    result.emplace_back(v, u);
                }
            }
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    SPDLOG_INFO("Successfully compute FAS, FAS number: {}, Time Elapsed: {}s", result.size(), duration / 1000.0);
    return result;
}