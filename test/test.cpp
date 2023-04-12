#include <gtest/gtest.h>

#include "base.h"
#define private public
#include "context.h"
#include "PageRankFAS.h"
#undef private

/**
 * 环检测单元测试
*/
TEST(CycleDetectionTest, CycleDetectionTest1) {
    // 创建Graph
    Graph g(5);
    boost::container::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {3, 1}, {4, 5}, {5, 6}, {6, 4}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }

    PageRankFAS fas;
    ASSERT_TRUE(fas.isCyclic(g));
}

TEST(CycleDetectionTest, CycleDetectionTest2) {
    // 创建Graph
    Graph g(5);
    boost::container::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }

    PageRankFAS fas;
    ASSERT_FALSE(fas.isCyclic(g));
}

TEST(CycleDetectionTest, CycleDetectionTest3) {
    // 创建Graph
    Graph g(5);
    boost::container::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 2}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }

    PageRankFAS fas;
    ASSERT_TRUE(fas.isCyclic(g));
}

/**
 * 强连通分量单元测试
*/
TEST(StrongConnectedComponentsTest, StrongConnectedComponentsTest1) {
    // 创建Graph
    Graph g(7);
    boost::container::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {5, 6}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }

    // 调用getConnectedComponents获取连通分量
    PageRankFAS fas;
    boost::container::vector<emhash8::HashSet<Vertex>> sccs;
    fas.computeStronglyConnectedComponents(g, sccs);
    ASSERT_EQ(sccs.size(), 3);
}

TEST(StrongConnectedComponentsTest, StrongConnectedComponentsTest2) {
    // 创建Graph
    Graph g(5);
    boost::container::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 2}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }

    // 调用getConnectedComponents获取连通分量
    PageRankFAS fas;
    boost::container::vector<emhash8::HashSet<Vertex>> sccs;
    fas.computeStronglyConnectedComponents(g, sccs);
    ASSERT_EQ(sccs.size(), 3);
}

TEST(StrongConnectedComponentsTest, StrongConnectedComponentsTest3) {
    // 创建Graph
    Graph g(5);
    boost::container::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 2}, {2, 1}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }

    // 调用getConnectedComponents获取连通分量
    PageRankFAS fas;
    boost::container::vector<emhash8::HashSet<Vertex>> sccs;
    fas.computeStronglyConnectedComponents(g, sccs);
    ASSERT_EQ(sccs.size(), 2);
}

/**
 * GreedyFAS算法单元测试
*/
TEST(MinFeedBackArcSetTest, GreedyFASTest1) {
    // 创建Graph
    Graph g;
    boost::container::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {3, 1}, {4, 5}, {5, 6}, {6, 4}};

    std::unordered_map<int, Vertex> vertex_map;
    for (const auto & edge: edges) {
        if (!vertex_map.count(edge.first)) {
            auto v = add_vertex(g);
            g[v] = edge.first;
            vertex_map[edge.first] = v;
        }
        if (!vertex_map.count(edge.second)) {
            auto v = add_vertex(g);
            g[v] = edge.second;
            vertex_map[edge.second] = v;
        }
    }

    for (const auto &edge : edges) {
        add_edge(vertex_map[edge.first], vertex_map[edge.second], g);
    }

    // 调用GreedyFAS获取最小反馈弧集
    FASContext context("greedy");
    auto feedback_arcs = context.getFeedbackArcSet(g);

    ASSERT_EQ(feedback_arcs.size(), 2);

    // 检查是否有环
    PageRankFAS fas;
    ASSERT_FALSE(fas.isCyclic(g));
}

TEST(MinFeedBackArcSetTest, GreedyFASTest2) {
    Graph g;
    boost::container::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 2}, {1, 3}, {2, 4}, {3, 0}, {4, 1}};

    std::unordered_map<int, Vertex> vertex_map;
    for (const auto & edge: edges) {
        if (!vertex_map.count(edge.first)) {
            auto v = add_vertex(g);
            g[v] = edge.first;
            vertex_map[edge.first] = v;
        }
        if (!vertex_map.count(edge.second)) {
            auto v = add_vertex(g);
            g[v] = edge.second;
            vertex_map[edge.second] = v;
        }
    }

    // 调用GreedyFAS获取最小反馈弧集
    FASContext context("greedy");
    auto feedback_arcs = context.getFeedbackArcSet(g);

    ASSERT_GE(feedback_arcs.size(), 3);
    ASSERT_LE(feedback_arcs.size(), 5);

    // 检查是否有环
    PageRankFAS fas;
    ASSERT_FALSE(fas.isCyclic(g));
}

TEST(MinFeedBackArcSetTest, GreedyFASTest3) {
    // 严格测试，需要至少去掉6条边才能没有环的图
    // 创建Graph
    Graph g;
    boost::container::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 2}, {1, 3}, {2, 4}, {3, 0}, {4, 1},
                                   {0, 5}, {1, 6}, {2, 7}, {3, 8}, {4, 9}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 5},
                                   {5, 7}, {6, 8}, {7, 9}, {8, 5}, {9, 6}};
    std::unordered_map<int, Vertex> vertex_map;
    for (const auto & edge: edges) {
        if (!vertex_map.count(edge.first)) {
            auto v = add_vertex(g);
            g[v] = edge.first;
            vertex_map[edge.first] = v;
        }
        if (!vertex_map.count(edge.second)) {
            auto v = add_vertex(g);
            g[v] = edge.second;
            vertex_map[edge.second] = v;
        }
    }

    for (const auto &edge : edges) {
        add_edge(vertex_map[edge.first], vertex_map[edge.second], g);
    }

    // 调用GreedyFAS获取最小反馈弧集
    FASContext context("greedy");
    auto feedback_arcs = context.getFeedbackArcSet(g);

    ASSERT_GE(feedback_arcs.size(), 6);
    ASSERT_LE(feedback_arcs.size(), 8);

    // 检查是否有环
    PageRankFAS fas;
    ASSERT_FALSE(fas.isCyclic(g));
}

TEST(MinFeedBackArcSetTest, GreedyFASTest4) {
    // 严格测试，需要至少去掉18条边才能没有环的图
    // 创建Graph
    Graph g;
    boost::container::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 2}, {1, 3}, {2, 4}, {3, 0}, {4, 1},
                                {0, 5}, {1, 6}, {2, 7}, {3, 8}, {4, 9}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 5},
                                {5, 7}, {6, 8}, {7, 9}, {8, 5}, {9, 6}, {0, 6}, {1, 7}, {2, 8}, {3, 9}, {4, 5},
                                {5, 1}, {6, 2}, {7, 3}, {8, 4}, {9, 0}, {0, 8}, {1, 9}, {2, 5}, {3, 6}, {4, 7},
                                {5, 3}, {6, 4}, {7, 0}, {8, 1}, {9, 2}};
    std::unordered_map<int, Vertex> vertex_map;
    for (const auto & edge: edges) {
        if (!vertex_map.count(edge.first)) {
            auto v = add_vertex(g);
            g[v] = edge.first;
            vertex_map[edge.first] = v;
        }
        if (!vertex_map.count(edge.second)) {
            auto v = add_vertex(g);
            g[v] = edge.second;
            vertex_map[edge.second] = v;
        }
    }

    for (const auto &edge : edges) {
        add_edge(vertex_map[edge.first], vertex_map[edge.second], g);
    }

    // 调用GreedyFAS获取最小反馈弧集
    FASContext context("greedy");
    auto feedback_arcs = context.getFeedbackArcSet(g);

    ASSERT_GE(feedback_arcs.size(), 15);
    ASSERT_LE(feedback_arcs.size(), 23);

    // 检查是否有环
    PageRankFAS fas;
    ASSERT_FALSE(fas.isCyclic(g));
}

/**
 * SortFAS算法单元测试
*/

TEST(MinFeedBackArcSetTest, SortFASTest1) {
    // 创建Graph
    Graph g;
    boost::container::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {3, 1}, {4, 5}, {5, 6}, {6, 4}};

    std::unordered_map<int, Vertex> vertex_map;
    for (const auto & edge: edges) {
        if (!vertex_map.count(edge.first)) {
            auto v = add_vertex(g);
            g[v] = edge.first;
            vertex_map[edge.first] = v;
        }
        if (!vertex_map.count(edge.second)) {
            auto v = add_vertex(g);
            g[v] = edge.second;
            vertex_map[edge.second] = v;
        }
    }

    for (const auto &edge : edges) {
        add_edge(vertex_map[edge.first], vertex_map[edge.second], g);
    }

    // 调用SortFAS获取最小反馈弧集
    FASContext context("sort");
    auto feedback_arcs = context.getFeedbackArcSet(g);

    ASSERT_EQ(feedback_arcs.size(), 2);
    // 检查是否有环
    PageRankFAS fas;
    ASSERT_FALSE(fas.isCyclic(g));
}

TEST(MinFeedBackArcSetTest,SortFASTest2) {
    // 创建Graph
    Graph g;
    boost::container::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 2}, {1, 3}, {2, 4}, {3, 0}, {4, 1}};

    std::unordered_map<int, Vertex> vertex_map;
    for (const auto & edge: edges) {
        if (!vertex_map.count(edge.first)) {
            auto v = add_vertex(g);
            g[v] = edge.first;
            vertex_map[edge.first] = v;
        }
        if (!vertex_map.count(edge.second)) {
            auto v = add_vertex(g);
            g[v] = edge.second;
            vertex_map[edge.second] = v;
        }
    }

    for (const auto &edge : edges) {
        add_edge(vertex_map[edge.first], vertex_map[edge.second], g);
    }

    // 调用SortFAS获取最小反馈弧集
    FASContext context("sort");
    auto feedback_arcs = context.getFeedbackArcSet(g);

    ASSERT_GE(feedback_arcs.size(), 3);
    ASSERT_LE(feedback_arcs.size(), 5);

    // 检查是否有环
    PageRankFAS fas;
    ASSERT_FALSE(fas.isCyclic(g));
}

TEST(MinFeedBackArcSetTest, SortFASTest3) {
    // 严格测试，需要至少去掉6条边才能没有环的图
    // 创建Graph
    Graph g;
    boost::container::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 2}, {1, 3}, {2, 4}, {3, 0}, {4, 1},
                                   {0, 5}, {1, 6}, {2, 7}, {3, 8}, {4, 9}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 5},
                                   {5, 7}, {6, 8}, {7, 9}, {8, 5}, {9, 6}};
    std::unordered_map<int, Vertex> vertex_map;
    for (const auto & edge: edges) {
        if (!vertex_map.count(edge.first)) {
            auto v = add_vertex(g);
            g[v] = edge.first;
            vertex_map[edge.first] = v;
        }
        if (!vertex_map.count(edge.second)) {
            auto v = add_vertex(g);
            g[v] = edge.second;
            vertex_map[edge.second] = v;
        }
    }

    for (const auto &edge : edges) {
        add_edge(vertex_map[edge.first], vertex_map[edge.second], g);
    }

    // 调用SortFAS获取最小反馈弧集
    FASContext context("sort");
    auto feedback_arcs = context.getFeedbackArcSet(g);

    ASSERT_GE(feedback_arcs.size(), 6);
    ASSERT_LE(feedback_arcs.size(), 8);

    // 检查是否有环
    PageRankFAS fas;
    ASSERT_FALSE(fas.isCyclic(g));
}

TEST(MinFeedBackArcSetTest, SortFASTest4) {
    // 严格测试，需要删除至少18条边才能使这个图没有环
    // 创建Graph
    Graph g;
    boost::container::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 2}, {1, 3}, {2, 4}, {3, 0}, {4, 1},
                                {0, 5}, {1, 6}, {2, 7}, {3, 8}, {4, 9}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 5},
                                {5, 7}, {6, 8}, {7, 9}, {8, 5}, {9, 6}, {0, 6}, {1, 7}, {2, 8}, {3, 9}, {4, 5},
                                {5, 1}, {6, 2}, {7, 3}, {8, 4}, {9, 0}, {0, 8}, {1, 9}, {2, 5}, {3, 6}, {4, 7},
                                {5, 3}, {6, 4}, {7, 0}, {8, 1}, {9, 2}};
    std::unordered_map<int, Vertex> vertex_map;
    for (const auto & edge: edges) {
        if (!vertex_map.count(edge.first)) {
            auto v = add_vertex(g);
            g[v] = edge.first;
            vertex_map[edge.first] = v;
        }
        if (!vertex_map.count(edge.second)) {
            auto v = add_vertex(g);
            g[v] = edge.second;
            vertex_map[edge.second] = v;
        }
    }

    for (const auto &edge : edges) {
        add_edge(vertex_map[edge.first], vertex_map[edge.second], g);
    }

    // 调用SortFAS获取最小反馈弧集
    FASContext context("sort");
    auto feedback_arcs = context.getFeedbackArcSet(g);

    ASSERT_GE(feedback_arcs.size(), 15);
    ASSERT_LE(feedback_arcs.size(), 23);
    
    // 检查是否有环
    PageRankFAS fas;
    ASSERT_FALSE(fas.isCyclic(g));
}

/**
 * PageRankFAS算法单元测试
*/
TEST(MinFeedBackArcSetTest, PageRankFASTest1) {
    // 创建Graph
    Graph g;
    boost::container::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {3, 1}, {4, 5}, {5, 6}, {6, 4}};

    std::unordered_map<int, Vertex> vertex_map;
    for (const auto & edge: edges) {
        if (!vertex_map.count(edge.first)) {
            auto v = add_vertex(g);
            g[v] = edge.first;
            vertex_map[edge.first] = v;
        }
        if (!vertex_map.count(edge.second)) {
            auto v = add_vertex(g);
            g[v] = edge.second;
            vertex_map[edge.second] = v;
        }
    }

    for (const auto &edge : edges) {
        add_edge(vertex_map[edge.first], vertex_map[edge.second], g);
    }

    // 调用PageRankFAS获取最小反馈弧集
    FASContext context("pagerank");
    auto feedback_arcs = context.getFeedbackArcSet(g);

    ASSERT_EQ(feedback_arcs.size(), 2);

    // 检查是否有环
    PageRankFAS fas;
    ASSERT_FALSE(fas.isCyclic(g));
}

TEST(MinFeedBackArcSetTest, PageRankFASTest2) {
    // 创建Graph
    Graph g;
    boost::container::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 2}, {1, 3}, {2, 4}, {3, 0}, {4, 1}};

    std::unordered_map<int, Vertex> vertex_map;
    for (const auto & edge: edges) {
        if (!vertex_map.count(edge.first)) {
            auto v = add_vertex(g);
            g[v] = edge.first;
            vertex_map[edge.first] = v;
        }
        if (!vertex_map.count(edge.second)) {
            auto v = add_vertex(g);
            g[v] = edge.second;
            vertex_map[edge.second] = v;
        }
    }

    for (const auto &edge : edges) {
        add_edge(vertex_map[edge.first], vertex_map[edge.second], g);
    }

    // 调用PageRankFAS获取最小反馈弧集
    FASContext context("pagerank");
    auto feedback_arcs = context.getFeedbackArcSet(g);

    ASSERT_GE(feedback_arcs.size(), 3);
    ASSERT_LE(feedback_arcs.size(), 5);

    // 检查是否有环
    PageRankFAS fas;
    ASSERT_FALSE(fas.isCyclic(g));
}

TEST(MinFeedBackArcSetTest, PageRankFASTest3) {
    // 严格测试，需要至少去掉6条边才能没有环的图
    // 创建Graph
    Graph g;
    boost::container::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 2}, {1, 3}, {2, 4}, {3, 0}, {4, 1},
                                   {0, 5}, {1, 6}, {2, 7}, {3, 8}, {4, 9}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 5},
                                   {5, 7}, {6, 8}, {7, 9}, {8, 5}, {9, 6}};
    std::unordered_map<int, Vertex> vertex_map;
    for (const auto & edge: edges) {
        if (!vertex_map.count(edge.first)) {
            auto v = add_vertex(g);
            g[v] = edge.first;
            vertex_map[edge.first] = v;
        }
        if (!vertex_map.count(edge.second)) {
            auto v = add_vertex(g);
            g[v] = edge.second;
            vertex_map[edge.second] = v;
        }
    }

    for (const auto &edge : edges) {
        add_edge(vertex_map[edge.first], vertex_map[edge.second], g);
    }

    // 调用PageRankFAS获取最小反馈弧集
    FASContext context("pagerank");
    auto feedback_arcs = context.getFeedbackArcSet(g);

    ASSERT_GE(feedback_arcs.size(), 6);
    ASSERT_LE(feedback_arcs.size(), 8);

    // 检查是否有环
    PageRankFAS fas;
    ASSERT_FALSE(fas.isCyclic(g));
}

TEST(MinFeedBackArcSetTest, PageRankFASTest4) {
    // 严格测试：需要删除至少15条边才能没有环
    // 创建Graph
    Graph g;
    boost::container::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 2}, {1, 3}, {2, 4}, {3, 0}, {4, 1},
                                {0, 5}, {1, 6}, {2, 7}, {3, 8}, {4, 9}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 5},
                                {5, 7}, {6, 8}, {7, 9}, {8, 5}, {9, 6}, {0, 6}, {1, 7}, {2, 8}, {3, 9}, {4, 5},
                                {5, 1}, {6, 2}, {7, 3}, {8, 4}, {9, 0}, {0, 8}, {1, 9}, {2, 5}, {3, 6}, {4, 7},
                                {5, 3}, {6, 4}, {7, 0}, {8, 1}, {9, 2}};
    std::unordered_map<int, Vertex> vertex_map;
    for (const auto & edge: edges) {
        if (!vertex_map.count(edge.first)) {
            auto v = add_vertex(g);
            g[v] = edge.first;
            vertex_map[edge.first] = v;
        }
        if (!vertex_map.count(edge.second)) {
            auto v = add_vertex(g);
            g[v] = edge.second;
            vertex_map[edge.second] = v;
        }
    }

    for (const auto &edge : edges) {
        add_edge(vertex_map[edge.first], vertex_map[edge.second], g);
    }

    // 调用PageRankFAS获取最小反馈弧集
    FASContext context("pagerank");
    auto feedback_arcs = context.getFeedbackArcSet(g);

    ASSERT_GE(feedback_arcs.size(), 15);
    ASSERT_LE(feedback_arcs.size(), 23);

    // 检查是否有环
    PageRankFAS fas;
    ASSERT_FALSE(fas.isCyclic(g));
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

