#include "SortFAS.h"
#include <numeric>
#include <random>

boost::container::vector<EdgePair> SortFAS::getFeedbackArcSet(Graph &g) {
    auto start = std::chrono::high_resolution_clock::now();
    SPDLOG_INFO("Starting SortFAS...");

    int n = boost::num_vertices(g);
    boost::container::vector<int> arrangement(n);
    emhash7::HashMap<int, Vertex> vertex_map;
    int cnt = 0;
    for (const auto &v : boost::make_iterator_range(boost::vertices(g))) {
        arrangement[cnt++] = g[v];
        vertex_map[g[v]] = v;
    }

    srand(time(nullptr));
    // 原数据是有序的，使用shuffle打乱顺序让评测更公平
    std::random_shuffle(arrangement.begin(), arrangement.end(), [](int i) {
        return rand() % i;
    });

    for (int i = 0; i < n; ++i) {
        int val = 0, min_val = 0, loc = i;
        int v = arrangement[i];
        const auto &vv = vertex_map.find(v);
        if (vv == vertex_map.end()) {
            continue;
        }
        for (int j = loc - 1; j >= 0; --j) {
            int w = arrangement[j];
            const auto &ww = vertex_map.find(w);
            if (ww == vertex_map.end()) {
                continue;
            }
            bool e_vw = false, e_wv = false;
            for (const auto &e : boost::make_iterator_range(boost::out_edges(vv->second, g))) {
                const auto &tgt = boost::target(e, g);
                if (tgt == ww->second) {
                    e_vw = true;
                    break;
                }
            }

            for (const auto &e : boost::make_iterator_range(boost::out_edges(ww->second, g))) {
                const auto &tgt = boost::target(e, g);
                if (tgt == vv->second) {
                    e_wv = true;
                    break;
                }
            }

            if (e_vw) {
                val -= 1;
            } else if (e_wv) {
                val += 1;
            }

            if (val <= min_val) {
                min_val = val;
                loc = j;
            }
        }

        // Insert v at position loc
        arrangement.erase(arrangement.begin() + i);
        arrangement.insert(arrangement.begin() + loc, v);
    }
    boost::container::vector<EdgePair> result;
    for (int i = 0; i < n - 1; ++i) {
        int v = arrangement[i];
        const auto &vv = vertex_map.find(v);
        if (vv == vertex_map.end()) {
            continue;
        }
        for (int j = i + 1; j < n; ++j) {
            int w = arrangement[j];
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
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    SPDLOG_INFO("Successfully compute FAS, FAS number: {}, Time Elapsed: {} s", result.size(), duration.count() / 1000.0);
    return result;
}