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
    auto components = fas.getConnectedComponents(g);

    ASSERT_EQ(components.size(), 3);

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
    auto components = fas.getConnectedComponents(g);

    ASSERT_EQ(components.size(), 3);
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
    auto components = fas.getConnectedComponents(g);

    ASSERT_EQ(components.size(), 2);

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
    auto feedback_arcs = context.getFAS(g);

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
    auto feedback_arcs = context.getFAS(g);

    ASSERT_EQ(feedback_arcs.size(), 2);
    ASSERT_EQ(source(feedback_arcs[0], g), 4);
    ASSERT_EQ(target(feedback_arcs[0], g), 1);
    ASSERT_EQ(source(feedback_arcs[1], g), 3);
    ASSERT_EQ(target(feedback_arcs[1], g), 0);
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

    // 调用GreedyFAS获取最小反馈弧集
    FASContext context("sort");
    auto feedback_arcs = context.getFAS(g);

    ASSERT_EQ(feedback_arcs.size(), 1);
    ASSERT_EQ(source(feedback_arcs[0], g), 4);
    ASSERT_EQ(target(feedback_arcs[0], g), 0);
}

TEST(MinFeedBackArcSetTest,SortFASTest2) {
    // 创建Graph
    Graph g(5);
    std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 2}, {1, 3}, {2, 4}, {3, 0}, {4, 1}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }

    // 调用GreedyFAS获取最小反馈弧集
    FASContext context("sort");
    auto feedback_arcs = context.getFAS(g);

    ASSERT_EQ(feedback_arcs.size(), 2);
    ASSERT_EQ(source(feedback_arcs[0], g), 4);
    ASSERT_EQ(target(feedback_arcs[0], g), 1);
    ASSERT_EQ(source(feedback_arcs[1], g), 3);
    ASSERT_EQ(target(feedback_arcs[1], g), 0);
}

/**
 * PageRankFAS算法单元测试
*/
/**
 * GreedyFAS算法单元测试
*/
TEST(MinFeedBackArcSetTest, PageRankFASTest1) {
    // 创建Graph
    Graph g(5);
    std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }

    // 调用GreedyFAS获取最小反馈弧集
    FASContext context("pagerank");
    auto feedback_arcs = context.getFAS(g);

    ASSERT_EQ(feedback_arcs.size(), 1);
    ASSERT_EQ(source(feedback_arcs[0], g), 4);
    ASSERT_EQ(target(feedback_arcs[0], g), 0);
}

TEST(MinFeedBackArcSetTest, PageRankFASTest2) {
    // 创建Graph
    Graph g(5);
    std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {0, 2}, {1, 3}, {2, 4}, {3, 0}, {4, 1}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }

    // 调用GreedyFAS获取最小反馈弧集
    FASContext context("pagerank");
    auto feedback_arcs = context.getFAS(g);

    ASSERT_EQ(feedback_arcs.size(), 2);
    ASSERT_EQ(source(feedback_arcs[0], g), 4);
    ASSERT_EQ(target(feedback_arcs[0], g), 1);
    ASSERT_EQ(source(feedback_arcs[1], g), 3);
    ASSERT_EQ(target(feedback_arcs[1], g), 0);
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

