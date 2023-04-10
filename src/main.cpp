/**
 * main - 程序入口
 * 
 * Usage: ./FASSolver <path/to/graph> [algorithm (greedy | sort | pagerank)] 
*/
#include "context.h"
#include <filesystem>
#include <boost/graph/graphviz.hpp>

namespace fs = std::filesystem;

void usage() {
    SPDLOG_ERROR("Usage: ./FASSolver <path/to/graph> [algorithm (greedy | sort | pagerank)]\n");
    exit(1);
}

void createDirectoryIfNotExists(const std::string &path) {
    fs::path dirPath(path);
    if (!fs::exists(dirPath)) {
        if (!fs::create_directory(dirPath)) {
            SPDLOG_ERROR("Failed to create directory: {}", path);
            exit(1);
        }
    } else if (!fs::is_directory(dirPath)) {
        SPDLOG_ERROR("Path exists, but is not a directory: {}", path);
        exit(1);
    }
}


void readGraph(const std::string &path, boost::container::vector<EdgePair> &edges) {
    FILE *fp = fopen(path.c_str(), "r");
    if (fp == nullptr) {
        SPDLOG_ERROR("Failed to open file: {}", path);
        exit(1);
    }
    char *line = nullptr;
    size_t len = 0;
    ssize_t read;
    while ((read = getline(&line, &len, fp)) != -1) {
        int u, v;
        sscanf(line, "%d,%d", &u, &v);
        edges.emplace_back(u, v);
    }
    fclose(fp);
}

void drawGraph(const std::string &path, Graph &g) {
    std::ofstream file(path);
    // 导出为graphviz格式
    boost::dynamic_properties dp;
    dp.property("node_id", boost::get(boost::vertex_bundle, g));
    boost::write_graphviz_dp(file, g, dp);
    file.close();
}

void writeResult(const std::string &path, boost::container::vector<EdgePair> &result) {
    // 如果不存在则创建
    FILE *fp = fopen(path.c_str(), "w");
    if (fp == nullptr) {
        SPDLOG_ERROR("Failed to open file: {}", path);
        exit(1);
    }
    fprintf(fp, "%lu\n", result.size());
    for (const auto &edge : result) {
        fprintf(fp, "%d,%d\n", edge.first, edge.second);
    }
    fclose(fp);
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