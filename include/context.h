/**
 * context.h - 上下文类
*/
#ifndef __CONTEXT_H__
#define __CONTEXT_H__
#include "GreedyFAS.h"
#include "SortFAS.h"
#include "PageRankFAS.h"

// 上下文类
class FASContext {
private:
    FASStrategy *strategy;
    bool isGreedy;
public:
    FASContext(FASStrategy *strategy) {
        this->strategy = strategy;
        isGreedy = false;
    }
    FASContext(std::string algorithm) {
        if (algorithm == "greedy") {
            isGreedy = true;
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
    boost::container::vector<EdgePair> getFeedbackArcSet(Graph& g) {
        if (isGreedy) {
            
        }
        return strategy->getFeedbackArcSet(g);
    }
};

#endif // __CONTEXT_H__