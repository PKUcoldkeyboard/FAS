/**
 * SortFAS.h - SortFAS Solver
*/
#ifndef __SORT_FAS_H__
#define __SORT_FAS_H__

#include "strategy.h"

class SortFAS: public FASStrategy {
public:
    std::vector<Edge> getFAS(Graph &g) override;
};

#endif // __SORT_FAS_H__