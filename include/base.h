/**
 * base.h 图数据类型封装和基本包引入
*/
#ifndef __BASE_H__
#define __BASE_H__
#include "spdlog_common.h"
#include <set>
#include <stack>
#include <vector>
#include <queue>
#include <unordered_map>
#include <string>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>

// 定义节点属性
typedef struct {
    int id;
} VertexProperty;

// 定义Graph数据类型: vecS -> 使用std::vector存储vertex和edge，bidirectionalS -> 可访问入边和出边的有向图
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::bidirectionalS,
        VertexProperty, boost::no_property> Graph;


// 定义Edge数据类型
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

// 定义Vertex数据类型
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

// 定义Edge Pair数据类型
typedef std::pair<int, int> EdgePair;

// 定义Vertex遍历器
typedef boost::graph_traits<Graph>::vertex_iterator vertex_iter;

// 定义Edge遍历器
typedef boost::graph_traits<Graph>::edge_iterator edge_iter;
typedef boost::graph_traits<Graph>::out_edge_iterator out_edge_iter;
typedef boost::graph_traits<Graph>::in_edge_iterator in_edge_iter;

// 定义Edge Pair的哈希函数
typedef struct {
    std::size_t operator()(const EdgePair &p) const {
        auto h1 = std::hash<int>{}(p.first);
        auto h2 = std::hash<int>{}(p.second);
        return h1 ^ h2;
    }
} EdgeHash;

// 线图G中的边映射为L(G)中的节点
typedef std::unordered_map<EdgePair, Vertex, EdgeHash> EdgeToVertexMap;
typedef std::unordered_map<Vertex, EdgePair> IdToEdgeMap;

#endif /* __BASE_H__ */