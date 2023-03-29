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
#include <string>
#include <iostream>
#include <boost/graph/adjacency_list.hpp>

// 定义Graph数据类型
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
        boost::no_property, boost::no_property> Graph;

// 定义Edge数据类型
typedef boost::graph_traits<Graph>::edge_descriptor Edge;

// 定义Vertex数据类型
typedef boost::graph_traits<Graph>::vertex_descriptor Vertex;

// 定义Edge Pair数据类型
typedef std::pair<int, int> EdgePair;

#endif /* __BASE_H__ */