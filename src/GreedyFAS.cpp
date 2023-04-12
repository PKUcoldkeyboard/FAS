#include "GreedyFAS.h"
#include <boost/graph/graph_utility.hpp>

// 定义GreedyGraph数据类型
typedef boost::adjacency_list<
        boost::vecS,                                 // 边容器
        boost::vecS,                                 // 节点容器
        boost::bidirectionalS,                       // 双向有向图
        int,                                         // 节点属性： 实际存储节点的id
        boost::no_property                           // 边属性： 无
> GreedyGraph;

constexpr Vertex INVALID_VERTEX = std::numeric_limits<Vertex>::max();

boost::container::vector<Vertex> get_source(const GreedyGraph &g) {
    boost::container::vector<Vertex> ret;
    for (const auto &v : boost::make_iterator_range(boost::vertices(g))) {
        if (boost::in_degree(v, g) == 0) {
            ret.emplace_back(v);
        }
    }
    return ret;
}

boost::container::vector<Vertex> get_sink(const GreedyGraph &g) {
    boost::container::vector<Vertex> ret;
    for (const auto &v : boost::make_iterator_range(boost::vertices(g))) {
        if (boost::out_degree(v, g) == 0) {
            ret.emplace_back(v);
        }
    }
    return ret;
}

bool empty(const GreedyGraph &g) {
    return boost::num_vertices(g) == 0;
}

boost::container::vector<EdgePair> GreedyFAS::getFeedbackArcSet(Graph &g) {
    SPDLOG_INFO("Starting GreedyFAS...");

    GreedyGraph gg;
    emhash7::HashMap<int, Vertex> vertex_map;

    // 需要重新构图
    for (const auto &e : boost::make_iterator_range(boost::edges(g))) {
        const auto &src = boost::source(e, g);
        const auto &tgt = boost::target(e, g);
        const auto &src_id = g[src];
        const auto &tgt_id = g[tgt];
        if (!vertex_map.count(src_id)) {
            auto v = add_vertex(src_id, gg);
            vertex_map[src_id] = v;
        }
        if (!vertex_map.count(tgt_id)) {
            auto v = add_vertex(tgt_id, gg);
            vertex_map[tgt_id] = v;
        }
    }

    for (const auto &e : boost::make_iterator_range(boost::edges(g))) {
        const auto &src = boost::source(e, g);
        const auto &tgt = boost::target(e, g);
        const auto &src_id = g[src];
        const auto &tgt_id = g[tgt];
        boost::add_edge(vertex_map[src_id], vertex_map[tgt_id], gg);
    }

    auto start = std::chrono::high_resolution_clock::now();

    boost::container::vector<int> s1, s2;
    boost::container::vector<EdgePair> result;

    srand(time(nullptr));

    while (!empty(gg)) {
        while (true) {
            const auto &source = get_source(gg);
            if (source.empty()) {
                break;
            }
            int idx = rand() % source.size();
            s2.insert(s2.begin(), gg[source[idx]]);
            boost::clear_vertex(idx, gg);
            boost::remove_vertex(idx, gg);
        }

        while (true) {
            const auto &sink = get_sink(gg);
            if (sink.empty()) {
                break;
            }
            int idx = rand() % sink.size();
            s1.emplace_back(gg[sink[idx]]);
            boost::clear_vertex(idx, gg);
            boost::remove_vertex(idx, gg);
        }

        int max_delta = -1;
        Vertex u = INVALID_VERTEX;
        for (const auto &v : boost::make_iterator_range(boost::vertices(gg))) {
            int delta = boost::out_degree(v, gg) - boost::in_degree(v, gg);
            if (delta > max_delta) {
                max_delta = delta;
                u = v;
            }
        }
        if (u != INVALID_VERTEX && !empty(gg)) {
            s1.push_back(gg[u]);
            boost::clear_vertex(u, gg);
            boost::remove_vertex(u, gg);
        }
    }
    s1.insert(s1.end(), s2.begin(), s2.end());

    for (int i = 0; i < s1.size() - 1; i++) {
        int v = s1[i];
        const auto &vv = vertex_map.find(v);
        if (vv == vertex_map.end()) {
            continue;
        }
        for (int j = i + 1; j < s1.size(); j++) {
            int w = s1[j];
            const auto &ww = vertex_map.find(w);
            if (ww == vertex_map.end()) {
                continue;
            }
            for (const auto &e : boost::make_iterator_range(boost::out_edges(ww->second, g))) {
                const auto &tgt = boost::target(e, g);
                if (tgt == vv->second) {
                    result.emplace_back(w, v);
                }
            }
        }
    }

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    SPDLOG_INFO("Successfully compute FAS, FAS number: {}, Time Elapsed: {}s", result.size(), duration / 1000.0);
    return result;
}