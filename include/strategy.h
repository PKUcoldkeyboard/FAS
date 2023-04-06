/**
 * strategy.h - 策略类
*/
#ifndef __STRATEGY_H__
#define __STRATEGY_H__
#include "base.h"

// 策略类
class FASStrategy {
public:
    /**
     * @brief 计算最小反馈弧集
     * @param g - 输入图
     * @return 最小反馈弧集，其中最小反馈弧用std::pair表示，如：(0,1),(1,2)...第一个数为起点，第二数为终点
    */
    virtual boost::container::vector<EdgePair> getFeedbackArcSet(Graph &g) = 0;
};

#endif // __STRATEGY_H__