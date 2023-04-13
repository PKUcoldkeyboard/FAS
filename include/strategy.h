/**
 * strategy.h - 策略类
*/
#ifndef __STRATEGY_H__
#define __STRATEGY_H__
#include <string>
#include <vector>

// 策略类
class FASStrategy {
public:
    virtual std::vector<std::pair<int, int>> getFeedbackArcSet(const std::string &input) = 0;
};

#endif // __STRATEGY_H__