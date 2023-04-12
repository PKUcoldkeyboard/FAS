#include "PageRankFAS.h"
#include <boost/graph/copy.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/reverse_graph.hpp>
#include <boost/graph/depth_first_search.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>
#include <chrono>

constexpr LineVertex INVALID_VERTEX = std::numeric_limits<LineVertex>::max();

bool dfs_cycle(const Graph &g, Vertex v, boost::container::vector<bool> &visited, boost::container::vector<bool> &on_stack) {
    visited[v] = true;
    on_stack[v] = true;

    for (const auto &ei : boost::make_iterator_range(boost::out_edges(v, g))) {
        const auto &u = boost::target(ei, g);
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

bool PageRankFAS::isCyclic(const Graph &g) {
    boost::container::vector<bool> visited(boost::num_vertices(g), false);
    boost::container::vector<bool> on_stack(boost::num_vertices(g), false);
    for (const auto &v : boost::make_iterator_range(boost::vertices(g))) {
        if (!visited[v]) {
            if (dfs_cycle(g, v, visited, on_stack))
                return true;
        }
    }
    return false;
}

boost::container::vector<EdgePair> PageRankFAS::getFeedbackArcSet(Graph &g) {
    boost::container::vector<EdgePair> feedback_arcs;
    // 提前预留feedback_arcs空间
    feedback_arcs.reserve(boost::num_edges(g) / 4);
    srand(time(nullptr));
    SPDLOG_INFO("Starting PageRankFAS...");

    int count = 0;
    bool flag = boost::num_vertices(g) > 10000;
    auto start = std::chrono::steady_clock::now();
    int duration = 0;

    while (isCyclic(g)) {
        // 打印图的节点数
        auto end = std::chrono::steady_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        // 每一次百次循环打印一次信息
        count++;
        if (count % 100 == 0) {
            SPDLOG_INFO("Graph is cyclic. Current FAS number: {}, Time Elapsed: {} s, Computing FAS...",
                        feedback_arcs.size(), duration / 1000.0);
        }
        // 求图G的强连通分量
        boost::container::vector<VertexHashSet> sccs;
        computeStronglyConnectedComponents(g, sccs);        

        // 为每个强连通分量创建线图
        for (auto &scc: sccs) {
            // 只有一个节点，直接跳过
            if (scc.size() == 1) {
                continue;
            }
            AdjacentEdgesMap adj_edge_map;
            VertexToEdgeMap vertex_to_edge_map;
            OutDegreeMap out_degree_map;
            LineGraph lg;
            // 计算强连通分量的线图
            boost::container::vector<bool> visited(boost::num_vertices(g), false);

            // 创建线图的顶点集并缓存出边
            for (const auto &u: scc) {
                for (const auto &ei: boost::make_iterator_range(boost::out_edges(u, g))) {
                    const auto &v = boost::target(ei, g);
                    if (scc.count(v)) {
                        const auto &z = boost::add_vertex({g[u], g[v]}, lg);
                        vertex_to_edge_map[z] = ei;
                        adj_edge_map[u].emplace_back(v, z);
                    }
                }
            }

            // 随机选取scc中的一个节点作为起点
            // auto s1 = std::chrono::steady_clock::now();
            const auto &v = *std::next(scc.begin(), rand() % scc.size());
            getLineGraph(g, lg, v, INVALID_VERTEX, visited, adj_edge_map, out_degree_map);
            // auto s2 = std::chrono::steady_clock::now();
            // auto d = std::chrono::duration_cast<std::chrono::milliseconds>(s2 - s1);
            // SPDLOG_INFO("Lg Time Elapsed: {} ms", d.count());

            // 计算线图的PageRank值
            // auto t1 = std::chrono::steady_clock::now();
            boost::container::vector<float> pagerank;
            computePageRank(lg, pagerank, out_degree_map);
            // auto t2 = std::chrono::steady_clock::now();
            // auto d2 = std::chrono::duration_cast<std::chrono::milliseconds>(t2 - t1);
            // SPDLOG_INFO("pr Time Elapsed: {} ms", d2.count());

            // // 打印pagerank
            // for (const auto &pr: pagerank) {
            //     SPDLOG_INFO("{}", pr);
            // }
            // // 打印分割线
            // SPDLOG_INFO("====================================");
            
            // 选取PageRank值最大的边
            const auto &max_pagerank = std::max_element(pagerank.begin(), pagerank.end());
            const auto &max_pagerank_index = std::distance(pagerank.begin(), max_pagerank);
            const auto &max_pagerank_edge = lg[max_pagerank_index];

            // 将选取的边加入feedback_arcs
            feedback_arcs.emplace_back(max_pagerank_edge);

            // 删除选取的边, 线图中的edge_pair存储了原图中两个节点的id对，所以可以直接删除，使用vertexToEdgeMap
            const auto &e = vertex_to_edge_map.at(max_pagerank_index);
            boost::remove_edge(e, g);
        }
    }
    // 释放多余的空间
    feedback_arcs.shrink_to_fit();
    SPDLOG_INFO("Successfully compute FAS, FAS number: {}, Time Elapsed: {} s", feedback_arcs.size(), duration / 1000.0);
    // 打印节点数
    SPDLOG_INFO("Graph remains {} nodes and {} edges.", boost::num_vertices(g), boost::num_edges(g));
    return feedback_arcs;
}

void PageRankFAS::getLineGraph(const Graph &g, LineGraph &lineGraph, const Vertex &v, const LineVertex &prev,
                               boost::container::vector<bool> &visited, const AdjacentEdgesMap &adj_edge_map,
                               OutDegreeMap &out_degree_map) {
    std::stack<std::pair<Vertex, LineVertex>> dfsStack;
    dfsStack.emplace(v, prev);

    while (!dfsStack.empty()) {
        const auto &curr = dfsStack.top();
        dfsStack.pop();
        const auto &curr_vertex = curr.first;
        const auto &curr_prev = curr.second;

        if (visited[curr_vertex]) {
            continue;
        }

        visited[curr_vertex] = true;
        const auto &v_adjacent_edges = adj_edge_map.at(curr_vertex);

        for (const auto &[u, z] : v_adjacent_edges) {
            if (curr_prev != INVALID_VERTEX && curr_prev != z) {
                // 存反边
                boost::add_edge(z, curr_prev, lineGraph);
                auto &out_degree = out_degree_map.at(curr_prev);
                out_degree++;
            }

            if (!visited[u]) {
                dfsStack.emplace(u, z);
            } else {
                const auto &u_adjacent_edges = adj_edge_map.at(u);

                for (const auto &[k, uk] : u_adjacent_edges) {
                    if (uk != z) {
                        boost::add_edge(uk, z, lineGraph);
                        auto &out_degree = out_degree_map[z];
                        out_degree++;
                    }
                }
            }
        }
    }
}

void computePageRankWorker(const LineGraph &lineGraph, const boost::container::vector<float> &old_pagerank,
                          boost::container::vector<float> &pagerank, const OutDegreeMap &out_degree_map, int start, int end) {
    for (auto vi = start; vi < end; ++vi) {
        float rank_sum = 0.0f;
        for (const auto &ei : boost::make_iterator_range(boost::out_edges(vi, lineGraph))) {
            const auto &u = boost::target(ei, lineGraph);
            const auto &n_out_edges = out_degree_map.at(u);
            if (n_out_edges > 0) {
                rank_sum += old_pagerank[u] / n_out_edges;
            }
        }
        pagerank[vi] = rank_sum;
    }
}

void PageRankFAS::computePageRank(const LineGraph &lineGraph, boost::container::vector<float> &pagerank, const OutDegreeMap &out_degree_map) {
    const int max_iterations = 5;
    const int num_threads = boost::thread::hardware_concurrency(); // 获取硬件支持的线程数

    pagerank.resize(boost::num_vertices(lineGraph), 1.0f / boost::num_vertices(lineGraph));
    boost::container::vector<float> old_pagerank(boost::num_vertices(lineGraph));

    boost::asio::thread_pool thread_pool(num_threads); // 使用 Boost.Asio 创建线程池

    for (unsigned int iter = 0; iter < max_iterations; iter++) {
        old_pagerank.swap(pagerank); // 使用 swap 函数提高访存效率

        int num_vertices_per_thread = boost::num_vertices(lineGraph) / num_threads;
        for (unsigned int i = 0; i < num_threads; ++i) {
            int start = i * num_vertices_per_thread;
            int end = (i == num_threads - 1) ? boost::num_vertices(lineGraph) : (i + 1) * num_vertices_per_thread;
            boost::asio::post(thread_pool, boost::bind(computePageRankWorker, boost::ref(lineGraph), boost::ref(old_pagerank), boost::ref(pagerank), boost::ref(out_degree_map), start, end));
        }

        thread_pool.join(); // 等待线程池中的所有任务完成
    }

    thread_pool.stop(); // 停止线程池
}

void tarjan(const Graph& G, Vertex v, boost::container::vector<int>& dfn, boost::container::vector<int>& low, boost::container::vector<bool>& on_stack, boost::container::vector<int>& scc, boost::container::vector<Vertex>& stack, int& index, int& scc_count) {
    dfn[v] = low[v] = ++index;
    on_stack[v] = true;
    stack.emplace_back(v);

    for (const auto &e : boost::make_iterator_range(boost::out_edges(v, G))) {
        const auto &w = boost::target(e, G);
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

void PageRankFAS::computeStronglyConnectedComponents(const Graph& g, boost::container::vector<VertexHashSet>& sccs) {
    // 利用 Tarjan 算法计算给定图中的强连接分量
    boost::container::vector<int> dfn(boost::num_vertices(g), -1);
    boost::container::vector<int> low(boost::num_vertices(g), -1);
    boost::container::vector<bool> on_stack(boost::num_vertices(g), false);
    boost::container::vector<int> scc(boost::num_vertices(g), -1);
    boost::container::vector<Vertex> stack;

    int index = 0;
    int scc_count = 0;
    for (auto v : boost::make_iterator_range(boost::vertices(g))) {
        if (scc[v] == -1) {
            tarjan(g, v, dfn, low, on_stack, scc, stack, index, scc_count);
        }
    }

    VertexHashSet tmp;
    sccs.resize(scc_count, tmp);
 
    for (auto v : boost::make_iterator_range(boost::vertices(g))) {
        sccs[scc[v]].insert(v);
    }
}