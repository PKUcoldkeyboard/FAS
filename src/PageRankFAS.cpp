#include "PageRankFAS.h"
#include <boost/graph/copy.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/reverse_graph.hpp>
#include <boost/graph/depth_first_search.hpp>

const Vertex INVALID_VERTEX = std::numeric_limits<Vertex>::max();

void dfs(const Graph &g, Vertex v, std::vector<bool>& visited, std::stack<Vertex> &stk) {
    visited[v] = true;
    out_edge_iter ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::out_edges(v, g); ei != ei_end; ++ei) {
        int u = boost::target(*ei, g);
        if (!visited[u]) {
            dfs(g, u, visited, stk);
        }
    }
    stk.push(v);
}

void dfs_reverse(const Graph &g, Vertex v, std::vector<bool> &visited, std::set<Vertex> &scc) {
    visited[v] = true;
    scc.insert(v);
    out_edge_iter ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::out_edges(v, g); ei != ei_end; ++ei) {
        int u = boost::target(*ei, g);
        if (!visited[u]) {
            dfs_reverse(g, u, visited, scc);
        }
    }
}

bool dfs_cycle(const Graph &g, Vertex v, std::vector<bool> &visited, std::vector<bool> &on_stack) {
    visited[v] = true;
    on_stack[v] = true;

    out_edge_iter ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::out_edges(v, g); ei != ei_end; ++ei) {
        Vertex u = boost::target(*ei, g);
        if (!visited[u]) {
            if (dfs_cycle(g, u, visited, on_stack))
                return true;
        }
        else if (on_stack[u]) {
            return true;
        }
    }

    on_stack[v] = false;
    return false;
}

// @TODO: fix this function
std::vector<EdgePair> PageRankFAS::getFeedbackArcSet(Graph &g) {
    std::vector<EdgePair> feedback_arcs;
    SPDLOG_INFO("Starting PageRankFAS...");

    while (isCyclic(g)) {
        SPDLOG_INFO("Graph is cyclic. Calculating the minimum feedback arc set...");
        // 求图G的强连通分量
        std::vector<std::set<Vertex>> sccs;
        computeStronglyConnectedComponents(g, sccs);

        int count = 0;
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
            std::vector<bool> visited(boost::num_vertices(g), false);
            // 随机选取scc中的一个节点作为起点
            srand(time(nullptr));
            auto v = *std::next(scc.begin(), rand() % scc.size());
            getLineGraph(g, lg, v, INVALID_VERTEX, visited, scc, edge_to_vertex_map, vertex_to_edge_map);

            // 计算线图的PageRank值
            std::vector<double> pagerank;
            computePageRank(lg, pagerank);
            
            // 选取PageRank值最大的边
            auto max_pagerank = std::max_element(pagerank.begin(), pagerank.end());
            auto max_pagerank_index = std::distance(pagerank.begin(), max_pagerank);
            auto max_pagerank_edge = lg[max_pagerank_index].edge_pair;

            // 将选取的边加入feedback_arcs
            feedback_arcs.push_back(max_pagerank_edge);

            // 删除选取的边, 线图中的edge_pair存储了原图中两个节点的id对，所以可以直接删除，使用vertexToEdgeMap
            auto e = vertex_to_edge_map[max_pagerank_index];
            boost::remove_edge(e, g);
        }
    }
    SPDLOG_INFO("Successfully calculate the minimum feedback arc set.");
    return feedback_arcs;
}


void PageRankFAS::getLineGraph(const Graph &g, LineGraph &lineGraph, Vertex v, Vertex prev, std::vector<bool> &visited, std::set<Vertex> &scc, EdgeToVertexMap &edge_to_vertex_map, VertexToEdgeMap &vertex_to_edge_map) {
    visited[v] = true;
    out_edge_iter ei, ei_end;
    // 遍历所有从v出发的边
    for (boost::tie(ei, ei_end) = boost::out_edges(v, g); ei != ei_end; ++ei) {
        auto u = boost::target(*ei, g);
        // 如果u不在scc中，直接跳过
        if (!scc.count(u)) {
            continue;
        }
        // 根据e=(v, u)找到L(G)中对应的节点索引
        auto e = std::make_pair(g[v].id, g[u].id);
        Vertex z;
        if (edge_to_vertex_map.count(e)) {
            z = edge_to_vertex_map[e];
        } else {
            z = boost::add_vertex(lineGraph);
            lineGraph[z].edge_pair = e;
            edge_to_vertex_map[e] = z;
            vertex_to_edge_map[z] = *ei;
        }

        if (prev != INVALID_VERTEX && prev != z) {
            boost::add_edge(prev, z, lineGraph);
        }

        if (!visited[u]) {
            getLineGraph(g, lineGraph, u, z, visited, scc, edge_to_vertex_map, vertex_to_edge_map);
        } else {
            out_edge_iter ej, ej_end;
            for (boost::tie(ej, ej_end) = boost::out_edges(u, g); ej != ej_end; ++ej) {
                auto k = boost::target(*ej, lineGraph);
                auto uk = edge_to_vertex_map[std::make_pair(u, k)];
                if (uk != z) {
                    boost::add_edge(z, uk, lineGraph);
                }
            }
        }
    }
}

void PageRankFAS::computePageRank(const LineGraph &lineGraph, std::vector<double> &pagerank) {
    const double dumping_factor = 0.85;
    const int max_iterations = 100;
    const double convergence_threshold = 1e-6;

    // key为顶点，value为索引, pagerank的大小为索引的最大值+1
    boost::property_map<LineGraph, boost::vertex_index_t>::type index_map = get(boost::vertex_index, lineGraph);

    // 初始化pagerank
    pagerank.resize(num_vertices(lineGraph), 1.0 / num_vertices(lineGraph));
    std::vector<double> old_pagerank(num_vertices(lineGraph));

    // 迭代计算pagerank
    for (int iter = 0; iter < max_iterations; iter++) {
        old_pagerank = pagerank;
        vertex_iter vi, vi_end;
        // 遍历线图的所有顶点
        for (boost::tie(vi, vi_end) = boost::vertices(lineGraph); vi != vi_end; vi++) {
            double rank_sum = 0;
            in_edge_iter ei, ei_end;
            // 遍历顶点vi的所有入边
            for (boost::tie(ei, ei_end) = boost::in_edges(*vi, lineGraph); ei != ei_end; ei++) {
                auto u = boost::source(*ei, lineGraph);
                auto n_out_edges = boost::out_degree(u, lineGraph);
                if (n_out_edges > 0) {
                    rank_sum += old_pagerank[index_map[u]] / n_out_edges;
                }
            }
            // 更新顶点vi的pagerank
            pagerank[index_map[*vi]] = dumping_factor * rank_sum + (1 - dumping_factor) / num_vertices(lineGraph);
        }

        double total_diff = 0.0;
        // 计算pagerank的总差值
        for (auto i = 0; i < pagerank.size(); i++) {
            total_diff += std::abs(pagerank[i] - old_pagerank[i]);
        }
        // 如果pagerank的总差值小于阈值，则停止迭代
        if (total_diff < convergence_threshold) {
            break;
        }
    }
}

bool PageRankFAS::isCyclic(Graph &g) {

    std::vector<bool> visited(boost::num_vertices(g), false);
    std::vector<bool> on_stack(boost::num_vertices(g), false);

    for (int i = 0; i < num_vertices(g); ++i) {
        if (!visited[i]) {
            if (dfs_cycle(g, i, visited, on_stack)) {
                return true;
            }
        }
    }
    return false;
}

// @TODO: fix this function
void PageRankFAS::computeStronglyConnectedComponents(Graph &g, std::vector<std::set<Vertex>> &sccs) {
    // 利用 Kosaraju 算法计算给定图中的强连接分量
    std::vector<bool> visited(boost::num_vertices(g), false);
    std::stack<Vertex> stk;
    for (int i = 0; i < boost::num_vertices(g); ++i) {
        if (!visited[i]) {
            dfs(g, i, visited, stk);
        }
    }

    Graph r;
    boost::copy_graph(boost::make_reverse_graph(g), r);
    visited.assign(boost::num_vertices(g), false);

    while (!stk.empty()) {
        auto v = stk.top();
        stk.pop();
        if (!visited[v]) {
            std::set<Vertex> scc;
            std::unordered_map<int, Vertex> vertex_map;
            dfs_reverse(r, v, visited, scc);
            sccs.push_back(scc);
        }
    }
}
