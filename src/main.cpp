/**
 * main - 程序入口
 * 
 * Usage: ./FASSolver <path/to/graph> [algorithm (greedy | sort | pagerank)] 
*/
#include <iostream>
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
    Graph g(5);
    std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 2}, {2, 1}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }
    // @TODO: 构造有向图
    auto result = context.getFAS(g);
    return 0;
}