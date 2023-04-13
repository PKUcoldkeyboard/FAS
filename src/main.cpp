/**
 * main - 程序入口
 * 
 * Usage: ./FASSolver <path/to/graph> [algorithm (greedy | sort | pagerank)] 
*/
#include "context.h"
#include "utils.hpp"
#include <boost/algorithm/string.hpp>

void usage() {
    SPDLOG_ERROR("Usage: ./FASSolver <path/to/graph> [algorithm (greedy | sort | pagerank)]\n");
    exit(1);
}

int main(int argc, char** argv) {
    if (argc != 3) {
        usage();
    }

    createDirectoryIfNotExists("result");

    fs::path graph_path(argv[1]);
    std::string algorithm(argv[2]);

    std::vector<std::string> graph_path_split;
    boost::split(graph_path_split, graph_path.string(), boost::is_any_of("/"));
    bool is_simple = false;
    if (graph_path_split[1] == "simple.txt") {
        is_simple = true;
    }
    std::vector<std::pair<int, int>> edges;
    if (is_simple) {
        int n = 0;
        read_edges(graph_path.string(), edges, n);
        draw_graph("result/simple.dot", edges);
    }

    FASContext context(algorithm);

    auto result = context.getFeedbackArcSet(graph_path.string());
    std::vector<std::string> result_str;
    boost::split(result_str, graph_path_split.back(), boost::is_any_of("."));
    write_result("result/" + result_str[0] + "_" + algorithm + ".txt", result);

    // 从edges中删除result
    if (is_simple) {
        for (const auto &edge : result) {
            edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
        }
        draw_graph("result/simple_after.dot", edges);
    }

    return 0;
}