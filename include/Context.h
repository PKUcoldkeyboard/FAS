/**
 * Context.h - 上下文类
*/
#ifndef __CONTEXT_H__
#define __CONTEXT_H__
#include "Strategy.h"
#include "Factory.h"

// 上下文类
class FASContext {
private:
    FASStrategy *strategy;
public:
    FASContext(FASStrategy *strategy) {
        this->strategy = strategy;
    }
    FASContext(std::string algorithm) {
        this->strategy = FASFactory::getStrategy(algorithm);
    }
    ~FASContext(){}
    void setStrategy(FASStrategy *strategy) {
        this->strategy = strategy;
    }
    std::vector<int> getFAS(Graph& g) {
        return strategy->getFAS(g);
    }
};

#endif // __CONTEXT_H__