/**
 * main - 程序入口
 * 
 * Usage: ./FASSolver <path/to/graph> [algorithm (greedy | sort | pagerank)] 
*/
#include "context.h"
#include "SimpleGraph.h"

void usage() {
    printf("Usage: ./FASSolver <path/to/graph> [algorithm (greedy | sort | pagerank)]\n");
    exit(0);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        usage();
    }
    std::string graph_path(argv[1]);
    std::string algorithm(argv[2]);
    
    FASContext context(algorithm);
    Graph g;
    // std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {3, 1}};
    std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 2}, {1, 3}, {2, 4}, {3, 0}, {4, 1},
                                   {0, 5}, {1, 6}, {2, 7}, {3, 8}, {4, 9}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 5},
                                   {5, 7}, {6, 8}, {7, 9}, {8, 5}, {9, 6}};
    std::unordered_map<int, Vertex> vertex_map;
    for (const auto & edge: edges) {
        if (!vertex_map.count(edge.first)) {
            auto v = add_vertex(g);
            g[v].id = edge.first;
            vertex_map[edge.first] = v;
        }
        if (!vertex_map.count(edge.second)) {
            auto v = add_vertex(g);
            g[v].id = edge.second;
            vertex_map[edge.second] = v;
        }
    }

    for (const auto &edge : edges) {
        add_edge(vertex_map[edge.first], vertex_map[edge.second], g);
    }

    auto result = context.getFeedbackArcSet(g);
    return 0;
}