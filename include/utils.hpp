#ifndef __UTILS_HPP__
#define __UTILS_HPP__
#include <filesystem>

namespace fs = std::filesystem;

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

void read_edges(std::string input, std::vector<std::pair<int, int>> &edges, int &n) {
    FILE* file = fopen(input.c_str(), "r");
    if (file == NULL) {
        SPDLOG_ERROR("Open file {} failed.", input);
    }
    int u, v;
    while (fscanf(file, "%d,%d", &u, &v) != EOF) {
        edges.emplace_back(std::make_pair(u, v));
        n = std::max(n, std::max(u, v));
    }
    n++;
    fclose(file);
}

void draw_graph(const std::string &path, std::vector<std::pair<int, int>> edges) {
    // 将edges转换为graphviz格式图
    std::string graphviz = "digraph G {\n";
    for (const auto &edge : edges) {
        graphviz += std::to_string(edge.first) + " -> " + std::to_string(edge.second) + ";\n";
    }
    graphviz += "}";
    // 如果不存在则创建
    FILE *fp = fopen(path.c_str(), "w");
    if (fp == nullptr) {
        SPDLOG_ERROR("Failed to open file: {}", path);
        exit(1);
    }
    fprintf(fp, "%s", graphviz.c_str());
    fclose(fp);
}

void write_result(const std::string &path, std::vector<std::pair<int, int>> &result) {
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

#endif // __UTILS_HPP__