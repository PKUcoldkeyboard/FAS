/**
 * Strategy.h - 策略类
*/
#ifndef __STRATEGY_H__
#define __STRATEGY_H__
#include <boost/graph/adjacency_list.hpp>
#include <vector>

// 定义Graph数据类型
typedef boost::adjacency_list<boost::vecS, boost::vecS, boost::directedS,
        boost::no_property, boost::no_property> Graph;

// 策略类
class FASStrategy {
public:
    // 寻找最小反馈弧集的接口
    virtual std::vector<int> getFAS(Graph &g) = 0;
};

#endif // __STRATEGY_H__