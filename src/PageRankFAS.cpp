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

    boost::graph_traits<Graph>::out_edge_iterator ei, ei_end;
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

std::vector<EdgePair> PageRankFAS::getFeedbackArcSet(Graph &g) {
    std::vector<EdgePair> feedback_arcs;
    SPDLOG_INFO("Starting PageRankFAS...");

    while (isCyclic(g)) {
        std::vector<Graph> sccs;
        getConnectedComponents(g, sccs);
        for (auto &scc : sccs) {
            Graph lineGraph;
            getLineGraph(scc, lineGraph);

            srand(time(NULL));
            auto v = rand() % boost::num_vertices(scc);

            std::vector<double> pagerank;
            calculatePageRank(lineGraph, pagerank);

            auto max_element_iter = std::max_element(pagerank.begin(), pagerank.end());
            auto u = std::distance(pagerank.begin(), max_element_iter);

            auto edge = id_to_edge_map[lineGraph[u].id];

            feedback_arcs.push_back(edge);

            boost::remove_edge(edge.first, edge.second, g);
        }
    }

    SPDLOG_INFO("Successfully calculate the minimum feedback arc set.");
    return feedback_arcs;
}

void PageRankFAS::getLineGraph(const Graph &g, Graph &lineGraph) {
    edge_iter ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::edges(g); ei != ei_end; ++ei) {
        auto u = boost::source(*ei, g);
        auto v = boost::target(*ei, g);
        auto z = boost::add_vertex(lineGraph);
        auto e = std::make_pair(u, v);
        edge_to_vertex_map[e] = z;
        id_to_edge_map[g[z].id] = e;
    }

    srand(time(NULL));
    auto v = rand() % boost::num_vertices(g);
    std::vector<bool> visited(boost::num_vertices(g), false);
    getLineGraph(g, lineGraph, v, INVALID_VERTEX, visited);
}

void PageRankFAS::getLineGraph(const Graph &g, Graph &lineGraph, Vertex v, Vertex prev, std::vector<bool> &visited) {
    // mark v as visited
    visited[v] = true;
    // for each edge e=(v,u) outgoing of v
    out_edge_iter ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::out_edges(v, g); ei != ei_end; ++ei) {
        Vertex u = boost::target(*ei, g);
        Vertex z = edge_to_vertex_map[std::make_pair(v, u)];
        
        // create an edge in L(G) from prev to v
        if (prev != INVALID_VERTEX && prev != z) {
            boost::add_edge(prev, z, lineGraph);
        }

        if (!visited[u]) {
            getLineGraph(g, lineGraph, u, z, visited);
        } else {
            // for each node k in L(G) that orginates from u, create an edge in L(G) from z to k
            out_edge_iter ej, ej_end;
            for (boost::tie(ej, ej_end) = boost::out_edges(u, lineGraph); ej != ej_end; ++ej) {
                Vertex k = boost::target(*ej, lineGraph);
                Vertex uk = edge_to_vertex_map[std::make_pair(u, k)];
                if (uk != z) {
                    boost::add_edge(z, uk, lineGraph);
                }
            }
        }
    }
}

void calculatePageRank(const Graph &lineGraph, std::vector<double> &pagerank) {
    const double dumping_factor = 0.85;
    const int max_iterations = 100;
    const double convergence_threshold = 1e-6;

    boost::property_map<Graph, boost::vertex_index_t>::type index_map = get(boost::vertex_index, lineGraph);

    pagerank.resize(num_vertices(lineGraph), 1.0 / num_vertices(lineGraph));
    std::vector<double> old_pagerank(num_vertices(lineGraph));

    for (int iter = 0; iter < max_iterations; iter++) {
        old_pagerank = pagerank;
        vertex_iter vi, vi_end;
        for (boost::tie(vi, vi_end) = boost::vertices(lineGraph); vi != vi_end; vi++) {
            double rank_sum = 0;
            in_edge_iter ei, ei_end;
            for (boost::tie(ei, ei_end) = boost::in_edges(*vi, lineGraph); ei != ei_end; ei++) {
                Vertex src = boost::source(*ei, lineGraph);
                auto n_out_edges = boost::out_degree(src, lineGraph);
                if (n_out_edges > 0) {
                    rank_sum += old_pagerank[index_map[src]] / n_out_edges;
                }
            }
            pagerank[index_map[*vi]] = (1.0 - dumping_factor) / boost::num_vertices(lineGraph) + dumping_factor * rank_sum;
        }

        double total_diff = 0.0;
        for (auto i = 0; i < pagerank.size(); i++) {
            total_diff += std::abs(pagerank[i] - old_pagerank[i]);
        }

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

void PageRankFAS::getConnectedComponents(Graph &g, std::vector<Graph> &sccs) {
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
    // 映射节点id(g[v].id)到连通分量中的节点

    while (!stk.empty()) {
        auto v = stk.top();
        stk.pop();
        if (!visited[v]) {
            std::set<Vertex> scc;
            std::unordered_map<int, Vertex> vertex_map;
            dfs_reverse(r, v, visited, scc);

            Graph scc_graph;
            for (auto &v : scc) {
                if (!vertex_map.count(g[v].id)) {
                    auto vertex = boost::add_vertex(scc_graph);
                    scc_graph[vertex].id = g[v].id;
                    vertex_map[g[v].id] = vertex;
                }
            }
            
            for (auto &v : scc) {
                out_edge_iter ei, ei_end;
                for (boost::tie(ei, ei_end) = boost::out_edges(v, g); ei != ei_end; ei++) {
                    auto t = boost::target(*ei, scc_graph);
                    if (vertex_map.count(g[t].id)) {
                        add_edge(vertex_map[g[v].id], vertex_map[g[t].id], scc_graph);
                    }
                }
            }
            sccs.push_back(scc_graph);
        }
    }
}


// std::vector<std::vector<int>> PageRankFAS::getConnectedComponents(Graph &g) {

//     // 利用 Kosaraju 算法计算给定图中的强连接分量
//     std::vector<std::vector<int>> ret;

//     // 在原图进行 DFS 遍历，并将节点按照时间排序
//     std::vector<bool> visited(boost::num_vertices(g), false);
//     std::stack<int> stk;

//     for (int i = 0; i < boost::num_vertices(g); ++i) {
//         if (!visited[i]) {
//             dfs(g, i, visited, stk);
//         }
//     }

//     // 在反转图上按照各个节点发现时间逆序进行 DFS 遍历
//     Graph r;
//     boost::copy_graph(boost::make_reverse_graph(g), r);
//     visited.assign(boost::num_vertices(g), false);

//     // std::cout << "Original Graph: " << std::endl;
//     // boost::print_graph(g, boost::get(boost::vertex_index, g));
//     // std::cout << "Reversed Graph " << std::endl;
//     // boost::print_graph(r, boost::get(boost::vertex_index, r));
    
//     while (!stk.empty()) {
//         auto v = stk.top();
//         stk.pop();
//         if (!visited[v]) {
// 	        std::vector<int> scc;
//             dfs_reverse(r, v, visited, scc);
//             ret.push_back(scc);
//         }
//     }

//     return ret;
// }
