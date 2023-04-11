/**
 * base.h 图数据类型封装和基本包引入
*/
#ifndef __BASE_H__
#define __BASE_H__
#include "spdlog_common.h"
#include "hash_table7.hpp"
#include "hash_set8.hpp"
#include <stack>
#include <vector>
#include <queue>
#include <string>
#include <iostream>
#include <boost/container/vector.hpp>
#include <boost/container/small_vector.hpp>
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
        boost::directedS,           // 有向图且只需要访问入边（存反图）
        EdgePair,                   // 节点属性： 实际存储边的起点和终点
        boost::no_property          // 边属性： 无
> LineGraph;

// 定义LineVertex数据类型
typedef boost::graph_traits<LineGraph>::vertex_descriptor LineVertex;

// 缓存某个节点u的所有出边v以及(u,v)对应的LineVertex
typedef emhash7::HashMap<Vertex, boost::container::small_vector<std::pair<Vertex, LineVertex>, 10>> AdjacentEdgesMap;

// 用于将线图L(G)的顶点映射为图G中的边, key为L(G)的顶点Vertex, value为图G中的边Edge，以便在图G中删除边
typedef emhash7::HashMap<LineVertex, Edge> VertexToEdgeMap;

// 存储线图的出度
typedef emhash7::HashMap<LineVertex, int> OutDegreeMap;

typedef emhash8::HashSet<Vertex> VertexHashSet;

#endif /* __BASE_H__ */