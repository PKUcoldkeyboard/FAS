/**
 * main - 程序入口
 * 
 * Usage: ./FASSolver <path/to/graph> [algorithm (greedy | sort | pagerank)] 
*/
#include "context.h"
#include <boost/graph/graphviz.hpp>

void usage() {
    printf("Usage: ./FASSolver <path/to/graph> [algorithm (greedy | sort | pagerank)]\n");
    exit(0);
}

void readGraph(const std::string &path, std::vector<EdgePair> &edges) {
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
        sscanf(line, "%d %d", &u, &v);
        edges.emplace_back(u, v);
    }
    fclose(fp);
}

void drawGraph(Graph &g) {
    std::ofstream file("result/graph.dot");
    // 导出为graphviz格式
    boost::dynamic_properties dp;
    dp.property("node_id", boost::get(&VertexProperty::id, g));
    boost::write_graphviz_dp(file, g, dp);
    file.close();
}

int main(int argc, char** argv) {
    if (argc != 3) {
        usage();
    }
    std::string graph_path(argv[1]);
    std::string algorithm(argv[2]);
    
    FASContext context(algorithm);
    Graph g;
    std::vector<EdgePair> edges;
    // 读取graph_path中的文本文件到edges中，每行是一条边
    readGraph(graph_path, edges);
    // 将edges中的边添加到图g中
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

    // 如果是简单图，生成graphviz格式
    if (graph_path == "graphs/simple.txt") {
        drawGraph(g);
    }
    // 测试一下输出结果
    SPDLOG_INFO("Feedback Arcs: ");
    for (auto &e : result) {
        SPDLOG_INFO("({}, {})", e.first, e.second);
    }

    return 0;
}