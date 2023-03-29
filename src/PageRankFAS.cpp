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

void dfs(const Graph& g, int v, std::vector<bool>& visited, std::stack<int> &stk) {
    visited[v] = true;
    boost::graph_traits<Graph>::out_edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::out_edges(v, g); ei != ei_end; ++ei)
    {
        int u = boost::target(*ei, g);
        if (!visited[u])
            dfs(g, u, visited, stk);
    }
    stk.push(v);
}

void dfs_reverse(const Graph& g, int v, std::vector<bool> &visited, std::vector<int> &scc) {
    visited[v] = true;
    scc.push_back(v);

    std::cout << v << " ";

    boost::graph_traits<Graph>::out_edge_iterator ei, ei_end;
    for (boost::tie(ei, ei_end) = boost::out_edges(v, g); ei != ei_end; ++ei)
    {
        int u = boost::target(*ei, g);
        if (!visited[u])
            dfs_reverse(g, u, visited, scc);
    }
}


std::vector<Edge> PageRankFAS::getFAS(Graph &g) {
    std::vector<Edge> feedback_arcs;
    log_info("Starting PageRankFAS...");

    std::vector<std::vector<int>> sccs = PageRankFAS::connected_components(g);
    for (int i = 0; i < sccs.size(); ++i) {
        std::cout << "SCC " << i << ": ";
        for (int j = 0; j < sccs[i].size(); ++j)
            std::cout << sccs[i][j] << " ";
        std::cout << std::endl;
    }

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

std::vector<std::vector<int>> PageRankFAS::connected_components(Graph &g) {

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
    /*
    // 利用 Kosaraju 算法计算给定图中的强连接分量
    //
    //
    //
    // 第一次DFS遍历，将节点按发现时间排序
    std::vector<int> discoverTime(boost::num_vertices(g));
    KosarajuDFS dfs1(discoverTime);
    boost::depth_first_search(g, boost::visitor(dfs1));
    
    // 对原图进行反转
    Graph r(boost::num_vertices(g));
    auto trans_map = boost::get(boost::vertex_index, r);
    auto reverse = boost::make_reverse_graph(g);
    auto orig_map = boost::get(boost::vertex_index, g);
    for (auto e : boost::make_iterator_range(boost::edges(reverse))) {
        boost::add_edge(boost::source(e, reverse), boost::target(e, reverse), r);
    }
    */
    //boost::print_graph(g, boost::get(boost::vertex_index, g));
    //boost::print_graph(r, boost::get(boost::vertex_index, r));
    
    // 在反转图上进行第一次 DFS
    //std::vector<int> discover_time(boost::num_vertices(g));
    //int t = 0;
    //boost::depth_first_search(reverse, boost::visitor(boost::make_dfs_visitor(boost::make_dfs_time_visitor(boost::make_iterator_property_map(discover_time.begin(), orig_map, 0, discover_time[0]), boost::on_discover_vertex([&](auto v, auto t) { discover_time[v] = t++; })))));
    /*
    // 在原图上按照发现时间进行排序
    std::vector<Graph::vertex_descriptor> sorted_vertices(boost::num_vertices(g));
    boost::copy(boost::vertices(g), sorted_vertices.begin());
    std::sort(sorted_vertices.begin(), sorted_vertices.end(), [&](auto u, auto v) { return discover_time[u] > discover_time[v]; });

    // 在原图上进行第二次 DFS
    std::vector<std::vector<Graph::vertex_descriptor>> sccs;
    std::vector<int> component(boost::num_vertices(g));
    boost::depth_first_search(g, boost::visitor(boost::make_dfs_visitor(boost::make_dfs_color_visitor(boost::make_iterator_property_map(component.begin(), orig_map, 0, component[0]), boost::on_finish_vertex([&](auto v, auto& g) { sccs.back().push_back(v); })))).root_vertex(sorted_vertices[0]));

    // 返回强连接分量
    return sccs;
    
    */

    //std::vector<int> ret;
    //return ret;
}
