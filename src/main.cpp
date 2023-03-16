/**
 * main - 程序入口
 * 
 * Usage: ./FASSolver <path/to/graph> [algorithm (greedy | sort | pagerank)] 
*/
#include <iostream>
#include "Context.h"

void usage() {
    printf("Usage: ./FASSolver <path/to/graph> [algorithm (greedy | sort | pagerank)]\n");
    exit(0);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        usage();
    }
    std::string graph_path = argv[1];
    std::string algorithm = argv[2];
    
    FASContext* context = new FASContext(algorithm);

    Graph g;
    // @TODO: 构造有向图
    std::vector<int> result = context->getFAS(g);
    return 0;
}