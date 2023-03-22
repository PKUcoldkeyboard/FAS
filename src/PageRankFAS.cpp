#include "PageRankFAS.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/range/irange.hpp>
#include <set>

std::vector<Edge> PageRankFAS::getFAS(Graph &g) {
    std::vector<Edge> feedback_arcs;
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

std::vector<int> PageRankFAS::connected_components(Graph &g) {
    std::vector<int> ret;
    return ret;
}
