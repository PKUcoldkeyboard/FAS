#include "PageRankFAS.h"
#include <boost/graph/graph_traits.hpp>
#include <boost/range/irange.hpp>
#include <set>

std::vector<int> PageRankFAS::getFAS(Graph &g) {
    std::vector<int> result;
    // @TODO: 实现PageRankFAS算法
    printf("Hello PageRankFAS\n");
    return result;
}

Graph* PageRankFAS::getLineGraph(Graph &g) {
    // Create a new graph L(G) of the same type as the input graph g
    Graph* lineGraph = new Graph(boost::num_vertices(g));

    // Iterate through all the vertices of the input graph g
    for (auto v : boost::make_iterator_range(boost::vertices(g))) {
        // Get the in-edges and out-edges of vertex v
        auto inEdges = boost::in_edges(v, g);
        auto outEdges = boost::out_edges(v, g);

        // Iterate through all pairs of in-edge e_i and out-edge e_j of vertex v
        for (auto e_i = inEdges.first; e_i != inEdges.second; ++e_i) {
            for (auto e_j = outEdges.first; e_j != outEdges.second; ++e_j) {
                // Add an edge in L(G) from e_i to e_j
                boost::add_edge(boost::source(*e_i, g), boost::target(*e_j, g), *lineGraph);
            }
        }
    }

    return lineGraph;
}

std::vector<double> PageRankFAS::calculatePageRank(Graph &lineGraph) {
    // Initialize a vector to store the PageRank values
    std::vector<double> pageRankValues(boost::num_vertices(lineGraph));

    // Set the damping factor (default value is 0.85)
    double dampingFactor = 0.85;

    // Set the number of iterations (default value is 100)
    int numIterations = 100;

    // Calculate the PageRank values using Boost's page_rank function
    boost::page_rank(lineGraph, boost::make_iterator_property_map(pageRankValues.begin(), get(boost::vertex_index, lineGraph)), boost::graph::n_iterations(numIterations), dampingFactor);

    return pageRankValues;
}

std::vector<int> PageRankFAS::calculateStrongComponents(Graph &g) {
    // Initialize a vector to store the component indices
    std::vector<int> componentIndices(boost::num_vertices(g));

    // Calculate the strongly connected components using Boost's strong_components function
    int numComponents = boost::strong_components(g, boost::make_iterator_property_map(componentIndices.begin(), get(boost::vertex_index, g)));

    return componentIndices;
}
