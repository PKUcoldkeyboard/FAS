#include "PageRankFAS.h"
#include "dbg.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/range/irange.hpp>
#include <set>

#undef check
#include <stack>
#include <boost/graph/copy.hpp>
#include <boost/graph/graph_utility.hpp>
#include <boost/graph/reverse_graph.hpp>
#include <boost/graph/depth_first_search.hpp>

void dfs(const Graph &g, int v, std::vector<bool>& visited, std::stack<int> &stk) {
    visited[v] = true;
    boost::graph_traits<Graph>::out_edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::out_edges(v, g); ei != ei_end; ++ei) {
        int u = boost::target(*ei, g);
        if (!visited[u])
            dfs(g, u, visited, stk);
    }
    stk.push(v);
}

void dfs_reverse(const Graph &g, int v, std::vector<bool> &visited, std::vector<int> &scc) {
    visited[v] = true;
    scc.push_back(v);

    std::cout << v << " ";

    boost::graph_traits<Graph>::out_edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::out_edges(v, g); ei != ei_end; ++ei) {
        int u = boost::target(*ei, g);
        if (!visited[u])
            dfs_reverse(g, u, visited, scc);
    }
}

bool dfs_cycle(const Graph &g, int v, std::vector<bool> &visited, std::vector<bool> &on_stack) {
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

std::vector<Edge> PageRankFAS::getFAS(Graph &g) {
    std::vector<Edge> feedback_arcs;
    log_info("Starting PageRankFAS...");

    std::vector<std::vector<int>> sccs = PageRankFAS::getConnectedComponents(g);
    for (int i = 0; i < sccs.size(); ++i) {
        std::cout << "SCC " << i << ": ";
        for (int j = 0; j < sccs[i].size(); ++j)
            std::cout << sccs[i][j] << " ";
        std::cout << std::endl;
    }

    if (PageRankFAS::isCyclic(g)) 
        std::cout << "Graph is cyclic." << std::endl;
    else
        std::cout << "Graph is not cyclic." << std::endl;

    // @TODO: 实现PageRankFAS算法
    return feedback_arcs;
}

Graph* PageRankFAS::getLineGraph(Graph &g) {
    return nullptr;
}

std::vector<double> PageRankFAS::calculatePageRank(Graph &lineGraph) {
    std::vector<double> ret;
    return ret;
}

std::vector<std::vector<int>> PageRankFAS::getConnectedComponents(Graph &g) {

    // 利用 Kosaraju 算法计算给定图中的强连接分量
    std::vector<std::vector<int>> ret;

    // 在原图进行 DFS 遍历，并将节点按照时间排序
    std::vector<bool> visited(boost::num_vertices(g), false);
    std::stack<int> stk;

    for (int i = 0; i < boost::num_vertices(g); ++i) {
        if (!visited[i])
            dfs(g, i, visited, stk);
    }

    // 在反转图上按照各个节点发现时间逆序进行 DFS 遍历
    Graph r;
    boost::copy_graph(boost::make_reverse_graph(g), r);
    visited.assign(boost::num_vertices(g), false);

    std::cout << "Original Graph: " << std::endl;
    boost::print_graph(g, boost::get(boost::vertex_index, g));
    std::cout << "Reversed Graph " << std::endl;
    boost::print_graph(r, boost::get(boost::vertex_index, r));
    
    while (!stk.empty()) {
        auto v = stk.top();
        stk.pop();
        if (!visited[v]) {
	    std::vector<int> scc;
            dfs_reverse(r, v, visited, scc);
            ret.push_back(scc);
	    std::cout<<std::endl;
        }
    }

    return ret;
}

bool PageRankFAS::isCyclic(Graph &g) {

    std::vector<bool> visited(boost::num_vertices(g), false);
    std::vector<bool> on_stack(boost::num_vertices(g), false);

    for (int i = 0; i < num_vertices(g); ++i) {
        if (!visited[i]) {
            if (dfs_cycle(g, i, visited, on_stack))
                return true;
        }
    }
}
