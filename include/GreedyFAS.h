/**
 * GreedyFAS.h - GreedyFAS Solver
*/
#ifndef __GREEDY_FAS_H__
#define __GREEDY_FAS_H__

#include "strategy.h"

class GreedyFAS: public FASStrategy {
public:
    std::vector<Edge> getFAS(Graph &g) override;
};

#endif // __GREEDY_FAS_H__