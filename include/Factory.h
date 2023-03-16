/**
 * Factory.h - 工厂类
*/
#ifndef __FACTORY_H__
#define __FACTORY_H__
#include "GreedyFAS.h"
#include "SortFAS.h"
#include "PageRankFAS.h"
#include <unordered_map>

// 策略工厂类
class FASFactory {
private:
    static std::unordered_map<std::string, FASStrategy*> strategies;
public:
    static FASStrategy* getStrategy(std::string algorithm) {
        if (strategies.count(algorithm) > 0) {
            return strategies[algorithm];
        } else {
            // 返回默认的PageRankStrategy
            return strategies["pagerank"];
        }
    }
};

// 初始化静态成员变量
std::unordered_map<std::string, FASStrategy*> FASFactory::strategies = {
        {"greedy", new GreedyFAS()},
        {"sort", new SortFAS()},
        {"pagerank", new PageRankFAS()}
};

#endif // __FACTORY_H__