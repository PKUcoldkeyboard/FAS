#include "PageRankFAS.h"
#include <boost/graph/copy.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/reverse_graph.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/thread.hpp>
#include <chrono>

const LineVertex INVALID_VERTEX = std::numeric_limits<LineVertex>::max();

bool dfs_cycle(const Graph &g, Vertex v, boost::container::vector<bool> &visited, boost::container::vector<bool> &on_stack) {
    visited[v] = true;
    on_stack[v] = true;

    for (auto ei : boost::make_iterator_range(boost::out_edges(v, g))) {
        Vertex u = boost::target(ei, g);
        if (!visited[u]) {
            if (dfs_cycle(g, u, visited, on_stack))
                return true;
        } else if (on_stack[u]) {
            return true;
        }
    }

    on_stack[v] = false;
    return false;
}

boost::container::vector<EdgePair> PageRankFAS::getFeedbackArcSet(Graph &g) {
    boost::container::vector<EdgePair> feedback_arcs;
    // 提前预留feedback_arcs空间
    feedback_arcs.reserve(boost::num_edges(g) / 5);
    SPDLOG_INFO("Starting PageRankFAS...");

    double timeElapsed = 0;
    int count = 0;
    while (isCyclic(g)) {
        // 打印图的节点数
        auto start = std::chrono::steady_clock::now();
        if (count % 10 == 0) {
            SPDLOG_INFO("Graph is cyclic. Current FAS number: {}, Time Elapsed: {} s, Calculating FAS...",
                        feedback_arcs.size(), timeElapsed / 1000);
        }
        // 求图G的强连通分量
        boost::container::vector<boost::unordered_set<Vertex>> sccs;
        computeStronglyConnectedComponents(g, sccs);

        // 为每个强连通分量创建线图
        for (auto &scc: sccs) {
            // 只有一个节点，直接跳过
            if (scc.size() == 1) {
                continue;
            }
            EdgeToVertexMap edge_to_vertex_map;
            VertexToEdgeMap vertex_to_edge_map;
            // 计算强连通分量的线图
            LineGraph lg;
            boost::container::vector<bool> visited(boost::num_vertices(g), false);

            // 创建线图的顶点集
            for (auto v: scc) {
                for (auto ei: boost::make_iterator_range(boost::out_edges(v, g))) {
                    auto u = boost::target(ei, g);
                    if (scc.count(u)) {
                        auto e = std::make_pair(g[v], g[u]);
                        auto z = boost::add_vertex(e, lg);
                        edge_to_vertex_map[e] = z;
                        vertex_to_edge_map[z] = ei;
                    }
                }
            }

            // 随机选取scc中的一个节点作为起点
            srand(time(nullptr));
            auto v = *std::next(scc.begin(), rand() % scc.size());
            getLineGraph(g, lg, v, INVALID_VERTEX, visited, scc, edge_to_vertex_map, vertex_to_edge_map);

            // 计算线图的PageRank值
            boost::container::vector<double> pagerank;
            computePageRank(lg, pagerank);
            
            // 选取PageRank值最大的边
            auto max_pagerank = std::max_element(pagerank.begin(), pagerank.end());
            auto max_pagerank_index = std::distance(pagerank.begin(), max_pagerank);
            auto max_pagerank_edge = lg[max_pagerank_index];

            // 将选取的边加入feedback_arcs
            feedback_arcs.emplace_back(max_pagerank_edge);

            // 删除选取的边, 线图中的edge_pair存储了原图中两个节点的id对，所以可以直接删除，使用vertexToEdgeMap
            auto e = vertex_to_edge_map[max_pagerank_index];
            boost::remove_edge(e, g);
        }
        auto end = std::chrono::steady_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        timeElapsed += duration.count();
        count++;
    }
    SPDLOG_INFO("Successfully compute FAS, FAS number: {}, Time Elapsed: {} s", feedback_arcs.size(), timeElapsed / 1000);
    // 清空多余分配的空间
    feedback_arcs.shrink_to_fit();
    return feedback_arcs;
}


void PageRankFAS::getLineGraph(const Graph &g, LineGraph &lineGraph, Vertex v, LineVertex prev,
                               boost::container::vector<bool> &visited, boost::unordered_set<Vertex> &scc,
                               EdgeToVertexMap &edge_to_vertex_map, VertexToEdgeMap &vertex_to_edge_map) {
    visited[v] = true;
    // 遍历所有从v出发的边
    for (auto ei : boost::make_iterator_range(boost::out_edges(v, g))) {
        auto u = boost::target(ei, g);
        // 如果u不在scc中，直接跳过
        if (!scc.count(u)) {
            continue;
        }
        // 根据e=(v, u)找到L(G)中对应的节点索引
        auto z = edge_to_vertex_map[std::make_pair(g[v], g[u])];

        if (prev != INVALID_VERTEX && prev != z) {
            boost::add_edge(prev, z, lineGraph);
        }

        if (!visited[u]) {
            getLineGraph(g, lineGraph, u, z, visited, scc, edge_to_vertex_map, vertex_to_edge_map);
        } else {
            for (auto ej : boost::make_iterator_range(boost::out_edges(u, g))) {
                auto k = boost::target(ej, g);
                if (!scc.count(k)) {
                    continue;
                }
                auto uk = edge_to_vertex_map[std::make_pair(g[u], g[k])];
                if (uk != z) {
                    boost::add_edge(z, uk, lineGraph);
                }
            }
        }
    }
}

void computePageRankWorker(const LineGraph &lineGraph, const boost::container::vector<double> &old_pagerank, boost::container::vector<double> &pagerank, int start, int end) {
    for (auto vi = start; vi < end; ++vi) {
        double rank_sum = 0;
        for (auto ei : boost::make_iterator_range(boost::in_edges(vi, lineGraph))) {
            auto u = boost::source(ei, lineGraph);
            auto n_out_edges = boost::out_degree(u, lineGraph);
            if (n_out_edges > 0) {
                rank_sum += old_pagerank[u] / n_out_edges;
            }
        }
        pagerank[vi] = rank_sum;
    }
}

void PageRankFAS::computePageRank(const LineGraph &lineGraph, boost::container::vector<double> &pagerank) {
    const int max_iterations = 5;
    const int num_threads = boost::thread::hardware_concurrency(); // 获取硬件支持的线程数

    pagerank.resize(num_vertices(lineGraph), 1.0 / num_vertices(lineGraph));
    boost::container::vector<double> old_pagerank(num_vertices(lineGraph));

    for (int iter = 0; iter < max_iterations; iter++) {
        old_pagerank = pagerank;

        boost::container::vector<boost::thread> threads;
        int num_vertices_per_thread = num_vertices(lineGraph) / num_threads;
        for (int i = 0; i < num_threads; ++i) {
            int start = i * num_vertices_per_thread;
            int end = (i == num_threads - 1) ? num_vertices(lineGraph) : (i + 1) * num_vertices_per_thread;
            threads.emplace_back(boost::thread(boost::bind(computePageRankWorker, boost::ref(lineGraph), boost::ref(old_pagerank), boost::ref(pagerank), start, end)));
        }

        for (auto &thread : threads) {
            thread.join();
        }
    }
}

bool PageRankFAS::isCyclic(Graph &g) {

    boost::container::vector<bool> visited(boost::num_vertices(g), false);
    boost::container::vector<bool> on_stack(boost::num_vertices(g), false);

    for (auto vi: boost::make_iterator_range(boost::vertices(g))) {
        if (!visited[vi]) {
            if (dfs_cycle(g, vi, visited, on_stack)) {
                return true;
            }
        }
    }
    return false;
}

void tarjan(Graph& G, Vertex v, boost::container::vector<int>& dfn, boost::container::vector<int>& low, boost::container::vector<bool>& on_stack, boost::container::vector<int>& scc, boost::container::vector<Vertex>& stack, int& index, int& scc_count) {
    dfn[v] = low[v] = ++index;
    on_stack[v] = true;
    stack.emplace_back(v);

    for (auto e : boost::make_iterator_range(boost::out_edges(v, G))) {
        auto w = boost::target(e, G);
        if (scc[w] == -1) {
            if (low[w] == -1) {
                tarjan(G, w, dfn, low, on_stack, scc, stack, index, scc_count);
                low[v] = std::min(low[v], low[w]);
            } else if (on_stack[w]) {
                low[v] = std::min(low[v], dfn[w]);
            }
        }
    }

    if (low[v] == dfn[v]) {
        auto w = stack.back();
        stack.pop_back();
        on_stack[v] = false;
        scc[v] = scc_count;
        while (w != v) {
            scc[w] = scc_count;
            w = stack.back();
            stack.pop_back();
            on_stack[w] = false;
        }
        scc_count++;
    }
}

void PageRankFAS::computeStronglyConnectedComponents(Graph& g, boost::container::vector<boost::unordered_set<Vertex>>& sccs) {
    // 利用 Tarjan 算法计算给定图中的强连接分量
    boost::container::vector<int> dfn(boost::num_vertices(g), -1);
    boost::container::vector<int> low(boost::num_vertices(g), -1);
    boost::container::vector<bool> on_stack(boost::num_vertices(g), false);
    boost::container::vector<int> scc(boost::num_vertices(g), -1);
    boost::container::vector<Graph::vertex_descriptor> stack;

    int index = 0;
    int scc_count = 0;
    for (auto v : boost::make_iterator_range(boost::vertices(g))) {
        if (scc[v] == -1) {
            tarjan(g, v, dfn, low, on_stack, scc, stack, index, scc_count);
        }
    }

    boost::unordered_set<Vertex> tmp;
    sccs.resize(scc_count, tmp);
 
    for (auto v : boost::make_iterator_range(boost::vertices(g))) {
        sccs[scc[v]].insert(v);
    }
}