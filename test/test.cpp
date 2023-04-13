#include <gtest/gtest.h>

#define private public
#include "context.h"
#include "PageRankFAS.hpp"
#include "utils.hpp"
#undef private

/**
 * 环检测单元测试
*/
TEST(CycGEDetectionTest, CycGEDetectionTest1) {
    // 创建Graph
    PageRankGraph g(7);
    std::vector<std::pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 0}, {3, 1}, {4, 5}, {5, 6}, {6, 4}};

    for (auto &edge : edges) {
        g.add_edge(edge.first, edge.second);
    }

    PageRankFAS fas;
    ASSERT_TRUE(fas.is_cyclic(g));
}

TEST(CycGEDetectionTest, CycGEDetectionTest2) {
    // 创建Graph
    PageRankGraph g(5);
    std::vector<std::pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}};

    for (auto &edge : edges) {
        g.add_edge(edge.first, edge.second);
    }

    PageRankFAS fas;
    ASSERT_FALSE(fas.is_cyclic(g));
}

TEST(CycGEDetectionTest, CycGEDetectionTest3) {
    // 创建Graph
    PageRankGraph g(5);
    std::vector<std::pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 2}};

    for (auto &edge : edges) {
        g.add_edge(edge.first, edge.second);
    }

    PageRankFAS fas;
    ASSERT_TRUE(fas.is_cyclic(g));
}

/**
 * 强连通分量单元测试
*/
TEST(StrongConnectedComponentsTest, StrongConnectedComponentsTest1) {
    // 创建Graph
    PageRankGraph g(7);
    std::vector<std::pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}, {5, 6}};

    for (auto &edge : edges) {
        g.add_edge(edge.first, edge.second);
    }

    // 调用getConnectedComponents获取连通分量
    PageRankFAS fas;
    std::vector<emhash8::HashSet<int>> sccs;
    fas.computeStronglyConnectedComponents(g, sccs);
    ASSERT_EQ(sccs.size(), 3);
}

TEST(StrongConnectedComponentsTest, StrongConnectedComponentsTest2) {
    // 创建Graph
    PageRankGraph g(5);
    std::vector<std::pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 2}};

    for (auto &edge : edges) {
        g.add_edge(edge.first, edge.second);
    }

    // 调用getConnectedComponents获取连通分量
    PageRankFAS fas;
    std::vector<emhash8::HashSet<int>> sccs;
    fas.computeStronglyConnectedComponents(g, sccs);
    ASSERT_EQ(sccs.size(), 3);
}

TEST(StrongConnectedComponentsTest, StrongConnectedComponentsTest3) {
    // 创建Graph
    PageRankGraph g(5);
    std::vector<std::pair<int, int>> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 2}, {2, 1}};

    for (auto &edge : edges) {
        g.add_edge(edge.first, edge.second);
    }

    // 调用getConnectedComponents获取连通分量
    PageRankFAS fas;
    std::vector<emhash8::HashSet<int>> sccs;
    fas.computeStronglyConnectedComponents(g, sccs);
    ASSERT_EQ(sccs.size(), 2);
}

/**
 * GreedyFAS算法单元测试
*/
TEST(MinFeedBackArcSetTest, GreedyFASTest1) {
    // 调用GreedyFAS获取最小反馈弧集
    FASContext context("greedy");
    auto feedback_arcs = context.getFeedbackArcSet("graphs/simple.txt");

    ASSERT_EQ(feedback_arcs.size(), 2);

    // 检查是否有环
    PageRankFAS fas;
    std::vector<std::pair<int, int>> edges;
    int n = 0;
    read_edges("graphs/simple.txt", edges, n);
    // 从edges删除feedback_arcs
    for (const auto &edge: feedback_arcs) {
        edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
    }
    PageRankGraph g(n);
    for (auto &edge : edges) {
        g.add_edge(edge.first, edge.second);
    }
    ASSERT_FALSE(fas.is_cyclic(g));
}

TEST(MinFeedBackArcSetTest, GreedyFASTest2) {
    // 调用GreedyFAS获取最小反馈弧集
    FASContext context("greedy");
    auto feedback_arcs = context.getFeedbackArcSet("graphs/graph1.txt");

    ASSERT_LE(feedback_arcs.size(), 25);

    // 检查是否有环
    PageRankFAS fas;
    std::vector<std::pair<int, int>> edges;
    int n = 0;
    read_edges("graphs/graph1.txt", edges, n);
    // 从edges删除feedback_arcs
    for (const auto &edge: feedback_arcs) {
        edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
    }
    PageRankGraph g(n);
    for (auto &edge : edges) {
        g.add_edge(edge.first, edge.second);
    }
    ASSERT_FALSE(fas.is_cyclic(g));
}

/**
 * SortFAS算法单元测试
*/

TEST(MinFeedBackArcSetTest, SortFASTest1) {
    FASContext context("sort");
    auto feedback_arcs = context.getFeedbackArcSet("graphs/simple.txt");

    ASSERT_EQ(feedback_arcs.size(), 2);

    // 检查是否有环
    PageRankFAS fas;
    std::vector<std::pair<int, int>> edges;
    int n = 0;
    read_edges("graphs/simple.txt", edges, n);
    // 从edges删除feedback_arcs
    for (const auto &edge: feedback_arcs) {
        edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
    }
    PageRankGraph g(n);
    for (auto &edge : edges) {
        g.add_edge(edge.first, edge.second);
    }
    ASSERT_FALSE(fas.is_cyclic(g));
}

TEST(MinFeedBackArcSetTest,SortFASTest2) {
    FASContext context("sort");
    auto feedback_arcs = context.getFeedbackArcSet("graphs/graph1.txt");

    ASSERT_LE(feedback_arcs.size(), 25);

    // 检查是否有环
    PageRankFAS fas;
    std::vector<std::pair<int, int>> edges;
    int n = 0;
    read_edges("graphs/graph1.txt", edges, n);
    // 从edges删除feedback_arcs
    for (const auto &edge: feedback_arcs) {
        edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
    }
    PageRankGraph g(n);
    for (auto &edge : edges) {
        g.add_edge(edge.first, edge.second);
    }
    ASSERT_FALSE(fas.is_cyclic(g));
}

/**
 * PageRankFAS算法单元测试
*/
TEST(MinFeedBackArcSetTest, PageRankFASTest1) {
    FASContext context("pagerank");
    auto feedback_arcs = context.getFeedbackArcSet("graphs/simple.txt");

    ASSERT_EQ(feedback_arcs.size(), 2);

    // 检查是否有环
    PageRankFAS fas;
    std::vector<std::pair<int, int>> edges;
    int n = 0;
    read_edges("graphs/simple.txt", edges, n);
    // 从edges删除feedback_arcs
    for (const auto &edge: feedback_arcs) {
        edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
    }
    PageRankGraph g(n);
    for (auto &edge : edges) {
        g.add_edge(edge.first, edge.second);
    }
    ASSERT_FALSE(fas.is_cyclic(g));
}

TEST(MinFeedBackArcSetTest, PageRankFASTest2) {
    FASContext context("pagerank");
    auto feedback_arcs = context.getFeedbackArcSet("graphs/graph1.txt");

    ASSERT_LE(feedback_arcs.size(), 25);

    // 检查是否有环
    PageRankFAS fas;
    std::vector<std::pair<int, int>> edges;
    int n = 0;
    read_edges("graphs/graph1.txt", edges, n);
    // 从edges删除feedback_arcs
    for (const auto &edge: feedback_arcs) {
        edges.erase(std::remove(edges.begin(), edges.end(), edge), edges.end());
    }
    PageRankGraph g(n);
    for (auto &edge : edges) {
        g.add_edge(edge.first, edge.second);
    }
    ASSERT_FALSE(fas.is_cyclic(g));
}

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

