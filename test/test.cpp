#include <gtest/gtest.h>

#include "base.h"
#define private public
#include "context.h"
#include "SimpleGraph.h"
#include "PageRankFAS.h"
#undef private

/**
 * 环检测单元测试
*/
TEST(CycleDetectionTest, CycleDetectionTest1) {
    // 创建Graph
    Graph g(5);
    std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }

    PageRankFAS fas;
    ASSERT_TRUE(fas.isCyclic(g));
}

TEST(CycleDetectionTest, CycleDetectionTest2) {
    // 创建Graph
    Graph g(5);
    std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }

    PageRankFAS fas;
    ASSERT_FALSE(fas.isCyclic(g));
}

TEST(CycleDetectionTest, CycleDetectionTest3) {
    // 创建Graph
    Graph g(5);
    std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 2}};

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
    std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {5, 6}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }

    // 调用getConnectedComponents获取连通分量
    PageRankFAS fas;
    std::vector<std::set<Vertex>> sccs;
    fas.computeStronglyConnectedComponents(g, sccs);
    ASSERT_EQ(sccs.size(), 3);
}

TEST(StrongConnectedComponentsTest, StrongConnectedComponentsTest2) {
    // 创建Graph
    Graph g(5);
    std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 2}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }

    // 调用getConnectedComponents获取连通分量
    PageRankFAS fas;
    std::vector<std::set<Vertex>> sccs;
    fas.computeStronglyConnectedComponents(g, sccs);
    ASSERT_EQ(sccs.size(), 3);
}

TEST(StrongConnectedComponentsTest, StrongConnectedComponentsTest3) {
    // 创建Graph
    Graph g(5);
    std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 2}, {2, 1}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }

    // 调用getConnectedComponents获取连通分量
    PageRankFAS fas;
    std::vector<std::set<Vertex>> sccs;
    fas.computeStronglyConnectedComponents(g, sccs);
    ASSERT_EQ(sccs.size(), 2);
}

/**
 * GreedyFAS算法单元测试
*/
TEST(MinFeedBackArcSetTest, GreedyFASTest1) {
    // 创建Graph
    Graph g(5);
    std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }

    // 调用GreedyFAS获取最小反馈弧集
    FASContext context("greedy");
    auto feedback_arcs = context.getFeedbackArcSet(g);

    ASSERT_EQ(feedback_arcs.size(), 1);
    ASSERT_EQ(source(feedback_arcs[0], g), 4);
    ASSERT_EQ(target(feedback_arcs[0], g), 0);
}

TEST(MinFeedBackArcSetTest, GreedyFASTest2) {
    // 创建Graph
    Graph g(5);
    std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 2}, {1, 3}, {2, 4}, {3, 0}, {4, 1}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }

    // 调用GreedyFAS获取最小反馈弧集
    FASContext context("greedy");
    auto feedback_arcs = context.getFeedbackArcSet(g);

    ASSERT_EQ(feedback_arcs.size(), 2);
    // ASSERT_EQ(source(feedback_arcs[0], g), 4);
    // ASSERT_EQ(target(feedback_arcs[0], g), 1);
    // ASSERT_EQ(source(feedback_arcs[1], g), 3);
    // ASSERT_EQ(target(feedback_arcs[1], g), 0);
}

/**
 * SortFAS算法单元测试
*/

TEST(MinFeedBackArcSetTest, SortFASTest1) {
    // 创建Graph
    Graph g(5);
    std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }

    // 调用SortFAS获取最小反馈弧集
    FASContext context("sort");
    auto feedback_arcs = context.getFeedbackArcSet(g);

    ASSERT_EQ(feedback_arcs.size(), 1);
    // ASSERT_EQ(source(feedback_arcs[0], g), 4);
    // ASSERT_EQ(target(feedback_arcs[0], g), 0);
}

TEST(MinFeedBackArcSetTest,SortFASTest2) {
    // 创建Graph
    Graph g(5);
    std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 2}, {1, 3}, {2, 4}, {3, 0}, {4, 1}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }

    // 调用SortFAS获取最小反馈弧集
    FASContext context("sort");
    auto feedback_arcs = context.getFeedbackArcSet(g);

    ASSERT_EQ(feedback_arcs.size(), 2);
    // ASSERT_EQ(source(feedback_arcs[0], g), 4);
    // ASSERT_EQ(target(feedback_arcs[0], g), 1);
    // ASSERT_EQ(source(feedback_arcs[1], g), 3);
    // ASSERT_EQ(target(feedback_arcs[1], g), 0);
}

/**
 * PageRankFAS算法单元测试
*/
/**
 * GreedyFAS算法单元测试
*/
TEST(MinFeedBackArcSetTest, PageRankFASTest1) {
    // 创建Graph
    Graph g;
    std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}};

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

    // 调用PageRankFAS获取最小反馈弧集
    FASContext context("pagerank");
    auto feedback_arcs = context.getFeedbackArcSet(g);

    ASSERT_EQ(feedback_arcs.size(), 1);
}

TEST(MinFeedBackArcSetTest, PageRankFASTest2) {
    // 创建Graph
    Graph g;
    std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 2}, {1, 3}, {2, 4}, {3, 0}, {4, 1}};

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

    // 调用PageRankFAS获取最小反馈弧集
    FASContext context("pagerank");
    auto feedback_arcs = context.getFeedbackArcSet(g);

    ASSERT_EQ(feedback_arcs.size(), 3);
}

TEST(MinFeedBackArcSetTest, PageRankFASTest3) {
    // 严格测试，需要至少去掉7条边才能没有环的图
    // 创建Graph
    Graph g;
    std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 2}, {1, 3}, {2, 4}, {3, 0}, {4, 1},
                                   {0, 5}, {1, 6}, {2, 7}, {3, 8}, {4, 9}, {5, 6}, {6, 7}, {7, 8}, {8, 9}, {9, 5},
                                   {5, 7}, {6, 8}, {7, 9}, {8, 5}, {9, 6}};
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

    // 调用PageRankFAS获取最小反馈弧集
    FASContext context("pagerank");
    auto feedback_arcs = context.getFeedbackArcSet(g);

    ASSERT_EQ(feedback_arcs.size(), 6);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

