/**
 * context.h - 上下文类
*/
#ifndef __CONTEXT_H__
#define __CONTEXT_H__
#include "GreedyFAS.hpp"
#include "SortFAS.hpp"
#include "PageRankFAS.hpp"

// 上下文类
class FASContext {
private:
    FASStrategy *strategy;
public:
    FASContext(FASStrategy *strategy) {
        this->strategy = strategy;
    }
    FASContext(std::string algorithm) {
        if (algorithm == "greedy") {
            this->strategy = new GreedyFAS(); 
        } else if (algorithm == "sort") {
            this->strategy = new SortFAS();
        } else {
            this->strategy = new PageRankFAS();
        }
    }
    ~FASContext(){
        delete strategy;
    }
    void setStrategy(FASStrategy *strategy) {
        this->strategy = strategy;
    }
    std::vector<std::pair<int, int>> getFeedbackArcSet(std::string input) {
        return strategy->getFeedbackArcSet(input);
    }
};

#endif // __CONTEXT_H__