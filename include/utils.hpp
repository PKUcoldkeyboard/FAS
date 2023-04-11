#ifndef __UTILS_HPP__
#define __UTILS_HPP__
#include "base.h"
#include <filesystem>
#include <boost/graph/graphviz.hpp>

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

#endif // __UTILS_HPP__