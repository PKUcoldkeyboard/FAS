/**
 * main - 程序入口
 * 
 * Usage: ./FASSolver <path/to/graph> [algorithm (greedy | sort | pagerank)] 
*/
#include "context.h"
#include "utils.hpp"

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
    
    FASContext context(algorithm);
    Graph g;
    boost::container::vector<EdgePair> edges;
    // 读取graph_path中的文本文件到edges中，每行是一条边
    readGraph(graph_path, edges);
    // 将edges中的边添加到图g中
    std::unordered_map<int, Vertex> vertex_map;
    for (const auto & edge: edges) {
        if (!vertex_map.count(edge.first)) {
            auto v = add_vertex(edge.first, g);
            vertex_map[edge.first] = v;
        }
        if (!vertex_map.count(edge.second)) {
            auto v = add_vertex(edge.second, g);
            vertex_map[edge.second] = v;
        }
    }

    for (const auto &edge : edges) {
        add_edge(vertex_map[edge.first], vertex_map[edge.second], g);
    }

    auto result = context.getFeedbackArcSet(g);
    
    // 如果是简单图，生成graphviz格式
    std::string graph_name = graph_path.stem().string();

    if (graph_name == "simple") {
        drawGraph("result/graph_before.dot", g);
    }

    // 输出结果到result目录
    std::string output_path = "result/" + graph_name + "_" + algorithm + "_"  + "result.txt";
    writeResult(output_path, result);

    return 0;
}