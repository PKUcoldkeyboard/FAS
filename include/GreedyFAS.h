/**
 * GreedyFAS.h - GreedyFAS Solver
*/
#ifndef __GREEDY_FAS_H__
#define __GREEDY_FAS_H__

#include "Strategy.h"

class GreedyFAS: public FASStrategy {
public:
    std::vector<int> getFAS(Graph &g) override;
};

#endif // __GREEDY_FAS_H__