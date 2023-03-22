/**
 * strategy.h - 策略类
*/
#ifndef __STRATEGY_H__
#define __STRATEGY_H__
#include "base.h"
#include <vector>

// 策略类
class FASStrategy {
public:
    // 寻找最小反馈弧集的接口
    virtual std::vector<Edge> getFAS(Graph &g) = 0;
};

#endif // __STRATEGY_H__