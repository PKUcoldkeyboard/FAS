#ifndef __PAGERANK_FAS_HPP__
#define __PAGERANK_FAS_HPP__
#include "strategy.h"
#include "spdlog_common.h"
#include "hash_table7.hpp"
#include "hash_set8.hpp"
#include "utils.hpp"
#include <boost/thread.hpp>
#include <boost/asio.hpp>
#include <random>
#include <stack>

typedef emhash7::HashMap<std::pair<int, int>, int, boost::hash<std::pair<int, int>>> EdgeMap;

class PageRankGraph {
public:
    int n;
    struct Edge {
        int to;
        int next;
    };
    PageRankGraph(int n) : n(n) {
        head.resize(n, -1);
    }
    std::vector<Edge> edges;
    std::vector<int> head;

    void add_edge(int u, int v) {
        edges.emplace_back(Edge{v, head[u]});
        head[u] = edges.size() - 1;
    }

    void remove_edge(int u, int v) {
        int prev = -1;
        for (int i = head[u]; i != -1; i = edges[i].next) {
            if (edges[i].to == v) {
                if (prev == -1) {
                    head[u] = edges[i].next;
                } else {
                    edges[prev].next = edges[i].next;
                }
                break;
            }
            prev = i;
        }
    }
};

class PageRankFAS : public FASStrategy {
public:
    std::vector<std::pair<int, int>> getFeedbackArcSet(const std::string &input) override {
        SPDLOG_INFO("Starting PageRankFAS...");
        std::vector<std::pair<int, int>> edges;
        int n = 0;
        read_edges(input, edges, n);
        PageRankGraph g(n);
        construct_graph(edges, g);

        std::vector<std::pair<int, int>> fas;
        std::vector<std::pair<int, int>>().swap(edges);

        // 开始算法
        auto start = std::chrono::steady_clock::now();
        int duration = 0;
        int cnt = 0;
        int old_size = 0;
        while (is_cyclic(g)) {
            auto end = std::chrono::steady_clock::now();
            duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
            // 每100次循环打印一次
            if ((cnt++) % 100 == 0) {
                SPDLOG_INFO("Graph is cyclic. Current FAS number: {}, Time Elapsed: {} s, Computing FAS...",
                            fas.size(), duration / 1000.0);
            }

            // 求图G的强连通分量
            std::vector<emhash8::HashSet<int>> sccs;
            computeStronglyConnectedComponents(g, sccs);    


            for (const auto &scc : sccs) {
                if (scc.size() <= 1) {
                    continue;
                }
                // 构建强连通分量的线图
                emhash7::HashMap<int, std::vector<int>> lineGraph;
                // 边索引
                EdgeMap edgeIndex;
                // 反向索引
                emhash7::HashMap<int, std::pair<int, int>> nodeIndex;

                // lineGraph的节点数
                int cnt = 0;
                // 添加节点
                for (const auto &v : scc) {
                    // 打印v
                    for (int i = g.head[v]; i != -1; i = g.edges[i].next) {
                        const auto &u = g.edges[i].to;
                        if (scc.count(u)) {
                            auto edge = std::make_pair(v, u);
                            lineGraph.emplace(cnt, std::vector<int>());
                            edgeIndex.emplace(edge, cnt);
                            nodeIndex.emplace(cnt, edge);
                            cnt++;
                        }
                    }
                }

                // 记录lineGraph节点的出度
                std::vector<int> out_degree(cnt, 0);
                
                // 执行线图算法
                unsigned seed = std::chrono::system_clock::now().time_since_epoch().count();
                std::default_random_engine generator(seed);
                std::uniform_int_distribution<int> distribution(0, scc.size() - 1);
                const auto &v = *std::next(scc.begin(), distribution(generator));
                std::vector<bool> visited(g.n, false);
                getLineGraph(g, lineGraph, v, -1, visited, out_degree, edgeIndex);

                // 执行pagerank算法
                std::vector<float> pagerank;
                computePageRank(lineGraph, pagerank, out_degree);

                // 选取pagerank中最大的节点
                int max_index = 0;
                float max_value = pagerank[0];
                for (int i = 1; i < pagerank.size(); i++) {
                    if (pagerank[i] > max_value) {
                        max_value = pagerank[i];
                        max_index = i;
                    }
                }

                // 根据节点索引找到lineGraph中的节点
                const auto &edge = nodeIndex[max_index];
                fas.emplace_back(edge);

                // 删除边
                g.remove_edge(edge.first, edge.second);;
            }
        }

        auto end = std::chrono::steady_clock::now();
        duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
        SPDLOG_INFO("Successfully compute FAS, FAS number: {}, Time Elapsed: {}s", fas.size(), duration / 1000.0);
        return fas;
    }
private:
    void construct_graph(const std::vector<std::pair<int, int>> &edges, PageRankGraph &g) {
        for (const auto &edge : edges) {
            g.add_edge(edge.first, edge.second);
        }
    }
    bool is_cyclic(const PageRankGraph &g) {
        std::vector<bool> visited(g.n, false);
        std::vector<bool> on_stack(g.n, false);
        for (int v = 0; v < g.n; v++) {
            if (visited[v] || g.head[v] == -1) {
                continue;
            }
            if (is_cyclic_util(g, v, visited, on_stack)) {
                return true;
            }
        }
        return false;
    }
    bool is_cyclic_util(const PageRankGraph &g, int v, std::vector<bool> &visited, std::vector<bool> &on_stack) {
        visited[v] = true;
        on_stack[v] = true;

        for (int i = g.head[v]; i != -1; i = g.edges[i].next) {
            const auto &u = g.edges[i].to;
            if (!visited[u]) {
                if (is_cyclic_util(g, u, visited, on_stack)) {
                    return true;
                }
            } else if (on_stack[u]) {
                return true;
            }
        }

        on_stack[v] = false;
        return false;
    }
    void tarjan(const PageRankGraph &g, int v, std::vector<int> &dfn, std::vector<int>& low, std::vector<bool>& on_stack, std::vector<int>& scc, std::vector<int>& stack, int& index, int& scc_count) {
        dfn[v] = low[v] = ++index;
        on_stack[v] = true;
        stack.emplace_back(v);

        for (int i = g.head[v]; i != -1; i = g.edges[i].next) {
            const auto &w = g.edges[i].to;
            if (scc[w] == -1) {
                if (low[w] == -1) {
                    tarjan(g, w, dfn, low, on_stack, scc, stack, index, scc_count);
                    low[v] = std::min(low[v], low[w]);
                } else if (on_stack[w]) {
                    low[v] = std::min(low[v], dfn[w]);
                }
            }
        }

        if (low[v] == dfn[v]) {
            auto w = stack.back();
            stack.pop_back();
            on_stack[v] = false;
            scc[v] = scc_count;
            while (w != v) {
                scc[w] = scc_count;
                w = stack.back();
                stack.pop_back();
                on_stack[w] = false;
            }
            scc_count++;
        }
    }
    void computeStronglyConnectedComponents(const PageRankGraph& g, std::vector<emhash8::HashSet<int>>& sccs) {
        std::vector<int> dfn(g.n, -1);
        std::vector<int> low(g.n, -1);
        std::vector<bool> on_stack(g.n, false);
        std::vector<int> scc(g.n, -1);
        std::vector<int> stack;
        int index = -1;
        int scc_count = 0;
        for (int v = 0; v < g.n; v++) {
            if (g.head[v] == -1) {
                continue;
            }
            if (scc[v] == -1) {
                tarjan(g, v, dfn, low, on_stack, scc, stack, index, scc_count);
            }
        }
        sccs.resize(scc_count);
        for (int v = 0; v < g.n; v++) {
            if (g.head[v] == -1) {
                continue;
            }
            sccs[scc[v]].insert(v);
        }
    }
    void getLineGraph(const PageRankGraph &g, emhash7::HashMap<int, std::vector<int>> &lineGraph,
                      const int &v, const int &prev, std::vector<bool> visited, std::vector<int> &out_degree,
                      const EdgeMap &edgeIndex) {
        std::stack<std::pair<int, int>> call_stack;
        call_stack.emplace(v, prev);

        while (!call_stack.empty()) {
            const auto &curr = call_stack.top();
            call_stack.pop();
            const auto &curr_v = curr.first;
            const auto &curr_prev = curr.second;

            if (visited[curr_v]) {
                continue;
            }
            visited[curr_v] = true;

            // 遍历curr_v的所有出边
            for (int i = g.head[curr_v]; i != -1; i = g.edges[i].next) {
                const auto &u = g.edges[i].to;
                const auto &z = edgeIndex.find({curr_v, u});
                if (z == edgeIndex.end()) {
                    continue;
                }
                if (curr_prev != -1 && curr_prev != z->second) {
                    // 存反边
                    lineGraph[z->second].emplace_back(curr_prev);
                    out_degree[curr_prev]++;
                }
                if (!visited[u]) {
                    call_stack.emplace(u, z->second);
                } else {
                    for (int j = g.head[u]; j != -1; j = g.edges[j].next) {
                        const auto &k = g.edges[j].to;
                        const auto &uk = edgeIndex.find({u, k});
                        if (uk == edgeIndex.end()) {
                            continue;
                        }
                        if (uk != z) {
                            // 存反边
                            lineGraph[uk->second].emplace_back(z->second);
                            out_degree[z->second]++;
                        }
                    }
                }
            }
        }
    }
    void computePageRankWorker(emhash7::HashMap<int, std::vector<int>> &lineGraph, std::vector<float> &old_pagerank,
                              std::vector<float> &pagerank, const std::vector<int> &out_degree, int start, int end) {
        for (auto i = start; i < end; i++) {
            float rank_sum = 0.0f;
            for (const auto &j : lineGraph[i]) {
                rank_sum += old_pagerank[j] / out_degree[j];
            }
            pagerank[i] = rank_sum;
        }
    }
    void computePageRank(emhash7::HashMap<int, std::vector<int>> &lineGraph, std::vector<float> &pagerank,
                         const std::vector<int> &out_degree) {
        
        const int max_iterations = 5;
        const int num_threads = boost::thread::hardware_concurrency(); // 获取硬件支持的线程数
        int n = out_degree.size();
        pagerank.resize(n, 1.0f / n);
        std::vector<float> old_pagerank(n);
        boost::asio::thread_pool thread_pool(num_threads);
        for (unsigned int iter = 0; iter < max_iterations; iter++) {
            old_pagerank.swap(pagerank); // 使用 swap 函数提高访存效率

            int num_vertices_per_thread = n / num_threads;
            for (unsigned int i = 0; i < num_threads; ++i) {
                int start = i * num_vertices_per_thread;
                int end = (i == num_threads - 1) ? n : (i + 1) * num_vertices_per_thread;
                boost::asio::post(thread_pool, boost::bind(&PageRankFAS::computePageRankWorker, this, boost::ref(lineGraph), boost::ref(old_pagerank),
                                boost::ref(pagerank), boost::ref(out_degree), start, end));
            }
            thread_pool.join(); // 等待线程池中的所有任务完成
        }
        thread_pool.stop(); // 停止线程池
    }
};

#endif // __PAGERANK_FAS_HPP__