#include <gtest/gtest.h>
#include "base.h"
#include "context.h"
#include "SimpleGraph.h"

TEST(MinFeedBackArcSetTest, GreedyFASTest1) {
    // 创建Graph
    Graph g(5);
    std::vector<EdgePair> edges = {{0, 1}, {1, 2}, {2, 3}, {3, 4}, {4, 0}};

    for (auto &edge : edges) {
        add_edge(edge.first, edge.second, g);
    }

    // 调用GreedyFAS获取最小反馈弧集
    auto context = std::make_unique<FASContext>("greedy");
    auto feedback_arcs = context->getFAS(g);

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
    auto context = std::make_unique<FASContext>("greedy");
    auto feedback_arcs = context->getFAS(g);

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

