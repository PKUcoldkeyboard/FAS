/**
 * base.h 图数据类型封装和基本包引入
*/
#ifndef __BASE_H__
#define __BASE_H__
#include "spdlog_common.h"
#include "hash_table8.hpp"
#include "hash_set8.hpp"
#include <stack>
#include <vector>
#include <queue>
#include <string>
#include <iostream>
#include <boost/container/vector.hpp>
#include <boost/graph/adjacency_list.hpp>

// 定义Edge Pair数据类型
typedef std::pair<int, int> EdgePair;

// 定义Graph数据类型
typedef boost::adjacency_list<
        boost::vecS,                                 // 边容器
        boost::vecS,                                 // 节点容器
        boost::directedS,                            // 有向图
        int,                                         // 节点属性： 实际存储节点的id
        boost::no_property                           // 边属性： 无
> Graph;

// 定义Edge数据类型
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

// 定义Vertex数据类型
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

// 定义LineGraph数据类型
typedef boost::adjacency_list<
        boost::vecS,                // 边容器
        boost::vecS,                // 节点容器
        boost::bidirectionalS,           // 有向图且需要访问入边（存反图）
        EdgePair,                   // 节点属性： 实际存储边的起点和终点
        boost::no_property          // 边属性： 无
> LineGraph;

// 定义LineVertex数据类型
typedef boost::graph_traits<LineGraph>::vertex_descriptor LineVertex;

// 定义Edge Pair的哈希函数
typedef boost::hash<std::pair<int, int>> EdgeHash;

// 用于图G中的边映射为线图L(G)的顶点, key为图G中边的表示EdgePair, value为L(G）中的顶点Vertex
typedef emhash8::HashMap<EdgePair, LineVertex, EdgeHash> EdgeToVertexMap;

// 用于将线图L(G)的顶点映射为图G中的边, key为L(G)的顶点Vertex, value为图G中的边Edge，以便在图G中删除边
typedef emhash8::HashMap<LineVertex, Edge, boost::hash<LineVertex>> VertexToEdgeMap;

#endif /* __BASE_H__ */